#ifdef ENABLE_API_VULKAN

#include "VulkanBackend.h"
#include "VulkanAPI.h"
#include "VulkanCommandList.h"
#include "VulkanDescriptorHeap.h"

namespace RHINO::APIVulkan {


    void VulkanBackend::Initialize() noexcept {
        VkApplicationInfo appInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        appInfo.pApplicationName = "RHINO";
        appInfo.pEngineName = "RHINO";

        VkInstanceCreateInfo instanceInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = 0;
        instanceInfo.enabledLayerCount = 0;
        vkCreateInstance(&instanceInfo, m_Alloc, &m_Instance);

        std::vector<VkPhysicalDevice> physicalDevices{};
        uint32_t physicalDevicesCount = 0;
        vkEnumeratePhysicalDevices(m_Instance, &physicalDevicesCount, nullptr);
        physicalDevices.resize(physicalDevicesCount);
        vkEnumeratePhysicalDevices(m_Instance, &physicalDevicesCount, physicalDevices.data());

        for (size_t i = 0; i < physicalDevices.size(); ++i) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(physicalDevices[i], &props);
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                m_PhysicalDevice = physicalDevices[i];
            }
        }
        m_PhysicalDevice = m_PhysicalDevice ? m_PhysicalDevice : physicalDevices[0];

        VkDeviceQueueCreateInfo queueInfos[3] = {};
        uint32_t queueInfosCount = 0;
        SelectQueues(queueInfos, &queueInfosCount);

        // PHYSICAL DEVICE FEATURES
        VkPhysicalDeviceDescriptorBufferFeaturesEXT deviceDescriptorBufferFeaturesExt{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT};
        deviceDescriptorBufferFeaturesExt.descriptorBuffer = VK_TRUE;
        deviceDescriptorBufferFeaturesExt.descriptorBufferPushDescriptors = VK_TRUE;

        VkPhysicalDeviceBufferDeviceAddressFeaturesEXT deviceAddressFeaturesExt{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES};
        deviceAddressFeaturesExt.pNext = &deviceDescriptorBufferFeaturesExt;
        deviceAddressFeaturesExt.bufferDeviceAddress = VK_TRUE;
        VkPhysicalDeviceFeatures2 deviceFeatures2{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
        deviceFeatures2.pNext = &deviceAddressFeaturesExt;
        deviceFeatures2.features = {};


        VkDeviceCreateInfo deviceInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
        deviceInfo.pNext = &deviceFeatures2;
        const char* deviceExtensions[] = {
                VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME,
                VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
        };
        deviceInfo.enabledExtensionCount = RHINO_ARR_SIZE(deviceExtensions);
        deviceInfo.ppEnabledExtensionNames = deviceExtensions;
        deviceInfo.queueCreateInfoCount = queueInfosCount;
        deviceInfo.pQueueCreateInfos = queueInfos;
        vkCreateDevice(m_PhysicalDevice, &deviceInfo, m_Alloc, &m_Device);

        LoadVulkanAPI(m_Instance, vkGetInstanceProcAddr);
    }

    void VulkanBackend::Release() noexcept {
        vkDestroyDevice(m_Device, m_Alloc);
        vkDestroyInstance(m_Instance, m_Alloc);
    }

    RTPSO* VulkanBackend::CompileRTPSO(const RTPSODesc& desc) noexcept {
        return nullptr;
    }

    void VulkanBackend::ReleaseRTPSO(RTPSO* pso) noexcept {
    }

    ComputePSO* VulkanBackend::CompileComputePSO(const ComputePSODesc& desc) noexcept {
        auto* result = new VulkanComputePSO{};

        VkPipelineLayoutCreateInfo layoutInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        layoutInfo.pushConstantRangeCount = 0;
        layoutInfo.setLayoutCount = 0;
        vkCreatePipelineLayout(m_Device, &layoutInfo, m_Alloc, &result->layout);

        VkShaderModuleCreateInfo shaderModuleInfo{};
        shaderModuleInfo.codeSize = desc.CS.bytecodeSize;
        shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(desc.CS.bytecode);
        vkCreateShaderModule(m_Device, &shaderModuleInfo, m_Alloc, &result->shaderModule);

        VkPipelineShaderStageCreateInfo stageInfo{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
        stageInfo.flags = 0;
        stageInfo.module = result->shaderModule;
        stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        stageInfo.pName = desc.CS.entrypoint;
        stageInfo.pSpecializationInfo = nullptr;

        VkComputePipelineCreateInfo createInfo{VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO};
        createInfo.stage = stageInfo;
        createInfo.layout = VK_NULL_HANDLE;
        vkCreateComputePipelines(m_Device, VK_NULL_HANDLE, 1, &createInfo, m_Alloc, &result->PSO);
        return result;
    }

    void VulkanBackend::ReleaseComputePSO(ComputePSO* pso) noexcept {
        auto* vulkanComputePSO = static_cast<VulkanComputePSO*>(pso);
        vkDestroyPipelineLayout(m_Device, vulkanComputePSO->layout, m_Alloc);
        vkDestroyPipeline(m_Device, vulkanComputePSO->PSO, m_Alloc);
        vkDestroyShaderModule(m_Device, vulkanComputePSO->shaderModule, m_Alloc);
        delete vulkanComputePSO;
    }

    Buffer* VulkanBackend::CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept {
        //TODO: use heapType and usage

        auto* result = new VulkanBuffer{};
        result->size = size;

        VkBufferCreateInfo createInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        createInfo.flags = 0;
        createInfo.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
        createInfo.size = size;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkCreateBuffer(m_Device, &createInfo, m_Alloc, &result->buffer);

        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(m_Device, result->buffer, &memReqs);
        VkPhysicalDeviceMemoryProperties memoryProps;
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memoryProps);

        VkMemoryAllocateFlagsInfo allocateFlagsInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO};
        allocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;

        VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        alloc.pNext = &allocateFlagsInfo;
        alloc.allocationSize = memReqs.size;
        alloc.memoryTypeIndex = SelectMemoryType(0xffffff, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkAllocateMemory(m_Device, &alloc, m_Alloc, &result->memory);

        vkBindBufferMemory(m_Device, result->buffer, result->memory, 0);

        VkBufferDeviceAddressInfo bufferInfo{VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO};
        bufferInfo.buffer = result->buffer;
        result->deviceAddress = vkGetBufferDeviceAddress(m_Device, &bufferInfo);

        return result;
    }

    void VulkanBackend::ReleaseBuffer(Buffer* buffer) noexcept {
    }

    Texture2D* VulkanBackend::CreateTexture2D() noexcept {
        return nullptr;
    }

    void VulkanBackend::ReleaseTexture2D(Texture2D* texture) noexcept {
    }

    DescriptorHeap* VulkanBackend::CreateDescriptorHeap(DescriptorHeapType type, size_t descriptorsCount, const char* name) noexcept {
        auto* result = new VulkanDescriptorHeap{};
        result->device = m_Device;

        VkPhysicalDeviceDescriptorBufferPropertiesEXT descriptorProps{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT};
        VkPhysicalDeviceProperties2 props{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
        props.pNext = &descriptorProps;
        vkGetPhysicalDeviceProperties2(m_PhysicalDevice, &props);

        result->descriptorProps = descriptorProps;

        result->descriptorHandleIncrementSize = CalculateDescriptorHandleIncrementSize(type, descriptorProps);

        result->heapSize = result->descriptorHandleIncrementSize * descriptorsCount;

        VkBufferCreateInfo heapCreateInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        heapCreateInfo.flags = 0;
        heapCreateInfo.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
        heapCreateInfo.size = result->heapSize;
        heapCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkCreateBuffer(m_Device, &heapCreateInfo, m_Alloc, &result->heap);

        // Create the memory backing up the buffer handle
        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(m_Device, result->heap, &memReqs);
        VkPhysicalDeviceMemoryProperties memoryProps;
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memoryProps);

        VkMemoryAllocateFlagsInfo allocateFlagsInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO};
        allocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;

        VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        alloc.pNext = &allocateFlagsInfo;
        alloc.allocationSize = memReqs.size;
        alloc.memoryTypeIndex = SelectMemoryType(0xffffff, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        vkAllocateMemory(m_Device, &alloc, m_Alloc, &result->memory);

        vkBindBufferMemory(m_Device, result->heap, result->memory, 0);

        VkBufferDeviceAddressInfo bufferInfo{VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO};
        bufferInfo.buffer = result->heap;
        result->heapGPUStartHandle = vkGetBufferDeviceAddress(m_Device, &bufferInfo);

        vkMapMemory(m_Device, result->memory, 0, VK_WHOLE_SIZE, 0, &result->mapped);
        return result;
    }

    void VulkanBackend::ReleaseDescriptorHeap(DescriptorHeap* heap) noexcept {
        auto* vulkanDescriptorHeap = static_cast<VulkanDescriptorHeap*>(heap);
        vkUnmapMemory(m_Device, vulkanDescriptorHeap->memory);
        vkDestroyBuffer(m_Device, vulkanDescriptorHeap->heap, m_Alloc);
        vkFreeMemory(m_Device, vulkanDescriptorHeap->memory, m_Alloc);
        delete heap;
    }

    CommandList* VulkanBackend::AllocateCommandList(const char* name) noexcept {
        return nullptr;
    }

    void VulkanBackend::ReleaseCommandList(CommandList* commandList) noexcept {
    }

    uint32_t VulkanBackend::SelectMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) noexcept {
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &deviceMemoryProperties);
        for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return std::numeric_limits<uint32_t>::max();
    }

    size_t VulkanBackend::CalculateDescriptorHandleIncrementSize(DescriptorHeapType heapType, const VkPhysicalDeviceDescriptorBufferPropertiesEXT& descriptorProps) noexcept {
        switch (heapType) {
            case DescriptorHeapType::SRV_CBV_UAV:
                return std::max(descriptorProps.uniformBufferDescriptorSize, std::max(descriptorProps.storageBufferDescriptorSize, std::max(descriptorProps.storageImageDescriptorSize, descriptorProps.sampledImageDescriptorSize)));
            case DescriptorHeapType::Sampler:
                return descriptorProps.samplerDescriptorSize;
            default:
                return 0;
        }
    }

    void VulkanBackend::SelectQueues(VkDeviceQueueCreateInfo queueInfos[3], uint32_t* infosCount) noexcept {
        uint32_t queuesCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queuesCount, nullptr);
        std::vector<VkQueueFamilyProperties> queues;
        queues.resize(queuesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queuesCount, queues.data());

        uint32_t graphicsQueueIndex = ~0;
        uint32_t computeQueueIndex = ~0;
        uint32_t copyQueueIndex = ~0;

        for (size_t i = 0; i < queuesCount; ++i) {
            if (queues[i].queueFlags & (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT) && queues[i].queueCount >= 3 && graphicsQueueIndex == ~0 && computeQueueIndex == ~0 && copyQueueIndex == ~0) {
                graphicsQueueIndex = i;
                computeQueueIndex = i;
                copyQueueIndex = i;
                break;
            } else if (queues[i].queueFlags & (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT) && queues[i].queueCount >= 2 && graphicsQueueIndex == ~0 && computeQueueIndex == ~0) {
                graphicsQueueIndex = i;
                computeQueueIndex = i;
            } else if (queues[i].queueFlags & (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT) && queues[i].queueCount >= 2 && computeQueueIndex == ~0 && copyQueueIndex == ~0) {
                computeQueueIndex = i;
                copyQueueIndex = i;
            } else if (queues[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT) && queues[i].queueCount >= 2 && graphicsQueueIndex == ~0 && copyQueueIndex == ~0) {
                graphicsQueueIndex = i;
                copyQueueIndex = i;
            } else if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsQueueIndex = i;
            } else if (queues[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                computeQueueIndex = i;
            } else if (queues[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
                copyQueueIndex = i;
            }
        }

        static const float priorities[3] = {0.5, 0.5, 0.5};
        if (graphicsQueueIndex == computeQueueIndex && computeQueueIndex == copyQueueIndex) {
            queueInfos[0].queueCount = 3;
            queueInfos[0].queueFamilyIndex = graphicsQueueIndex;
            queueInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[0].pQueuePriorities = priorities;
            *infosCount = 1;
        } else if (graphicsQueueIndex == computeQueueIndex) {
            *infosCount = 2;

            queueInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[0].queueCount = 2;
            queueInfos[0].queueFamilyIndex = graphicsQueueIndex;
            queueInfos[0].pQueuePriorities = priorities;

            queueInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[1].queueCount = 1;
            queueInfos[1].queueFamilyIndex = copyQueueIndex;
            queueInfos[1].pQueuePriorities = priorities;
        } else if (graphicsQueueIndex == copyQueueIndex) {
            *infosCount = 2;

            queueInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[0].queueCount = 2;
            queueInfos[0].queueFamilyIndex = graphicsQueueIndex;
            queueInfos[0].pQueuePriorities = priorities;

            queueInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[1].queueCount = 1;
            queueInfos[1].queueFamilyIndex = computeQueueIndex;
            queueInfos[1].pQueuePriorities = priorities;
        } else if (computeQueueIndex == copyQueueIndex) {
            *infosCount = 2;

            queueInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[0].queueCount = 2;
            queueInfos[0].queueFamilyIndex = computeQueueIndex;
            queueInfos[0].pQueuePriorities = priorities;

            queueInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[1].queueCount = 1;
            queueInfos[1].queueFamilyIndex = graphicsQueueIndex;
            queueInfos[1].pQueuePriorities = priorities;
        } else {
            *infosCount = 3;

            queueInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[0].queueCount = 1;
            queueInfos[0].queueFamilyIndex = graphicsQueueIndex;
            queueInfos[0].pQueuePriorities = priorities;

            queueInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[1].queueCount = 1;
            queueInfos[1].queueFamilyIndex = computeQueueIndex;
            queueInfos[1].pQueuePriorities = priorities;

            queueInfos[2].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[2].queueCount = 1;
            queueInfos[2].queueFamilyIndex = computeQueueIndex;
            queueInfos[2].pQueuePriorities = priorities;
        }
    }

}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN