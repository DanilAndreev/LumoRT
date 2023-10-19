export module VulkanBackend;

import VulkanBackendTypes;

namespace RHINO::APIVulkan {
    export class VulkanBackend final : public RHINOInterface {
    public:
        explicit VulkanBackend() noexcept = default;

    public:
        void Initialize() noexcept final {
        }
        void Release() noexcept final {
        }

    public:
        RTPSO* CompileRTPSO(const RTPSODesc& desc) noexcept final {
            return nullptr;
        }
        void ReleaseRTPSO(RTPSO* pso) noexcept final {
        }
        ComputePSO* CompileComputePSO(const ComputePSODesc& desc) noexcept final {
            return nullptr;
        }
        void ReleaseComputePSO(ComputePSO* pso) noexcept final {
        }

        Buffer* CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept final {
            return nullptr;
        }
        void ReleaseBuffer(Buffer* buffer) noexcept final {
        }
        Texture2D* CreateTexture2D() noexcept final {
            return nullptr;
        }
        void ReleaseTexture2D(Texture2D* texture) noexcept final {
        }
        DescriptorHeap* CreateDescriptorHeap(DescriptorHeapType type, size_t descriptorsCount, const char* name) noexcept final {
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
        void ReleaseDescriptorHeap(DescriptorHeap* heap) noexcept final {
            auto* vulkanDescriptorHeap = static_cast<VulkanDescriptorHeap*>(heap);
            vkUnmapMemory(m_Device, vulkanDescriptorHeap->memory);
            vkDestroyBuffer(m_Device, vulkanDescriptorHeap->heap, m_Alloc);
            vkFreeMemory(m_Device, vulkanDescriptorHeap->memory, m_Alloc);
            delete heap;
        }

        CommandList* AllocateCommandList(const char* name) noexcept final {
            return nullptr;
        }
        void ReleaseCommandList(CommandList* commandList) noexcept final {
        }
        void DispatchCompute() noexcept final {
        }
        void DispatchComputeIndirect() noexcept final {
        }
        void TraceRays() noexcept final {
        }

    private:
        uint32_t SelectMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) noexcept {
            VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};
            vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &deviceMemoryProperties);
            for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) && (deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }
            return std::numeric_limits<uint32_t>::max();
        }

    private:
        VkDevice m_Device = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice;
        VkAllocationCallbacks* m_Alloc = nullptr;
    };
}// namespace RHINO::APIVulkan
