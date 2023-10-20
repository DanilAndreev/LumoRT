#ifdef ENABLE_API_VULKAN

#include "VulkanBackend.h"
// #include "VulkanAPI.h"

namespace RHINO::APIVulkan {


    void VulkanBackend::Initialize() noexcept {
        // LoadVulkanAPI(m_Instance);
    }
    void VulkanBackend::Release() noexcept {
    }

    RTPSO* VulkanBackend::CompileRTPSO(const RTPSODesc& desc) noexcept {
        return nullptr;
    }
    void VulkanBackend::ReleaseRTPSO(RTPSO* pso) noexcept {
    }
    ComputePSO* VulkanBackend::CompileComputePSO(const ComputePSODesc& desc) noexcept {
        return nullptr;
    }
    void VulkanBackend::ReleaseComputePSO(ComputePSO* pso) noexcept {
    }

    Buffer* VulkanBackend::CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept {
        return nullptr;
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
        VkPhysicalDeviceDescriptorBufferPropertiesEXT propsEXT{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT};
        VkPhysicalDeviceProperties2 props{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
        props.pNext = &propsEXT;
        vkGetPhysicalDeviceProperties2(m_PhysicalDevice, &props);

        result->descriptorHandleIncrementSize = propsEXT.storageBufferDescriptorSize;

        assert(propsEXT.storageBufferDescriptorSize == propsEXT.uniformBufferDescriptorSize);
        VkBufferCreateInfo heapCreateInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        heapCreateInfo.flags = 0;
        heapCreateInfo.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;
        heapCreateInfo.size = result->descriptorHandleIncrementSize * descriptorsCount;
        heapCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkCreateBuffer(m_Device, &heapCreateInfo, m_Alloc, &result->heap);

        // Create the memory backing up the buffer handle
        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(m_Device, result->heap, &memReqs);
        VkPhysicalDeviceMemoryProperties memoryProps;
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memoryProps);

        VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        alloc.allocationSize = memReqs.size;
        alloc.memoryTypeIndex = SelectMemoryType(0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
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
    void VulkanBackend::DispatchCompute() noexcept {
    }
    void VulkanBackend::DispatchComputeIndirect() noexcept {
    }
    void VulkanBackend::TraceRays() noexcept {
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

}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN