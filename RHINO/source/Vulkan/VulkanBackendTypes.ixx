export module VulkanBackendTypes;

namespace RHINO::APIVulkan {
    export class VulkanBuffer : public Buffer {
    public:
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceAddress deviceAddress = 0;
    };

    export class VulkanTexture2D : public Texture2D {
    public:
        VkImage texture = VK_NULL_HANDLE;
    };

    export class VulkanDescriptorHeap : public  DescriptorHeap {
    public:
        void WriteSRV(const WriteBufferSRVDesc& desc) noexcept final {
            auto* vulkanBuffer = static_cast<VulkanBuffer*>(desc.buffer);

            VkDescriptorAddressInfoEXT bufferInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_ADDRESS_INFO_EXT};
            bufferInfo.address = vulkanBuffer->deviceAddress + desc.bufferOffset;
            bufferInfo.range = VK_WHOLE_SIZE;
            VkDescriptorGetInfoEXT info{VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT};
            info.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            info.data.pStorageBuffer = &bufferInfo;

            uint8_t* mem = static_cast<uint8_t*>(mapped);
            vkGetDescriptorEXT(device, &info, descriptorHandleIncrementSize, mem + desc.offsetInHeap * descriptorHandleIncrementSize);
        }
        void WriteUAV(const WriteBufferSRVDesc& desc) noexcept final {
            auto* vulkanBuffer = static_cast<VulkanBuffer*>(desc.buffer);

            VkDescriptorAddressInfoEXT bufferInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_ADDRESS_INFO_EXT};
            bufferInfo.address = vulkanBuffer->deviceAddress + desc.bufferOffset;
            bufferInfo.range = VK_WHOLE_SIZE;
            VkDescriptorGetInfoEXT info{VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT};
            info.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            info.data.pStorageBuffer = &bufferInfo;

            uint8_t* mem = static_cast<uint8_t*>(mapped);
            vkGetDescriptorEXT(device, &info, descriptorHandleIncrementSize, mem + desc.offsetInHeap * descriptorHandleIncrementSize);
        }
        void WriteCBV(const WriteBufferSRVDesc& desc) noexcept final {
            auto* vulkanBuffer = static_cast<VulkanBuffer*>(desc.buffer);

            VkDescriptorAddressInfoEXT bufferInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_ADDRESS_INFO_EXT};
            bufferInfo.address = vulkanBuffer->deviceAddress + desc.bufferOffset;
            bufferInfo.range = VK_WHOLE_SIZE;
            VkDescriptorGetInfoEXT info{VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT};
            info.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            info.data.pUniformBuffer = &bufferInfo;

            uint8_t* mem = static_cast<uint8_t*>(mapped);
            vkGetDescriptorEXT(device, &info, descriptorHandleIncrementSize, mem + desc.offsetInHeap * descriptorHandleIncrementSize);

        }
        void WriteSRV(const WriteTexture2DSRVDesc& desc) noexcept final {
            auto* vulkanTexture = static_cast<VulkanTexture2D*>(desc.texture);

            VkDescriptorImageInfo textureInfo{};
//            textureInfo.imageLayout = ;
//            textureInfo.imageView = ;
            VkDescriptorGetInfoEXT info{VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT};
            info.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            info.data.pSampledImage = &textureInfo;

            uint8_t* mem = static_cast<uint8_t*>(mapped);
            vkGetDescriptorEXT(device, &info, descriptorHandleIncrementSize, mem + desc.offsetInHeap * descriptorHandleIncrementSize);

        }
        void WriteUAV(const WriteTexture2DSRVDesc& desc) noexcept final {
        }

        void WriteSRV(const WriteTexture3DSRVDesc& desc) noexcept final {
        }
        void WriteUAV(const WriteTexture3DSRVDesc& desc) noexcept final {
        }
    public:
        VkBuffer heap = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        void* mapped = nullptr;
        VkDeviceAddress heapGPUStartHandle = 0;
        size_t descriptorHandleIncrementSize = 0;

        VkDevice device = VK_NULL_HANDLE;
    };

    export class VulkanRTPSO : public RTPSO {
    public:
    };

    export class VulkanComputePSO : public ComputePSO {
    public:
        ID3D12RootSignature* rootSignature = nullptr;
        ID3D12PipelineState* PSO = nullptr;
    };

    export class VulkanCommandList : public CommandList {
    public:
        void Dispatch(const DispatchDesc& desc) noexcept final {
        }

        void Draw() noexcept final {
        }

        void SetHeap(DescriptorHeap* heap) noexcept final {
            auto* vulkanDescriptorHeap = static_cast<VulkanDescriptorHeap*>(heap);
            VkDescriptorBufferBindingInfoEXT binding{VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT};
            binding.address = vulkanDescriptorHeap->heapGPUStartHandle;
            binding.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;
            vkCmdBindDescriptorBuffersEXT(cmd, 1, &binding);
            //vulkanDescriptorHeap->heap;
        }
    private:
        VkCommandBuffer cmd = VK_NULL_HANDLE;
    };
}
