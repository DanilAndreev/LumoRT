#pragma once

#ifdef ENABLE_API_VULKAN

#include "VulkanBackendTypes.h"

namespace RHINO::APIVulkan {
    class VulkanDescriptorHeap final : public DescriptorHeap {
    public:
        void WriteSRV(const WriteBufferSRVDesc& desc) noexcept final;
        void WriteUAV(const WriteBufferSRVDesc& desc) noexcept final;
        void WriteCBV(const WriteBufferSRVDesc& desc) noexcept final;
        void WriteSRV(const WriteTexture2DSRVDesc& desc) noexcept final;
        void WriteUAV(const WriteTexture2DSRVDesc& desc) noexcept final;
        void WriteSRV(const WriteTexture3DSRVDesc& desc) noexcept final;
        void WriteUAV(const WriteTexture3DSRVDesc& desc) noexcept final;

    public:
        VkDevice device = VK_NULL_HANDLE;
        VkDescriptorPool pool = VK_NULL_HANDLE;
        VkDescriptorSet heap = VK_NULL_HANDLE;
    };

}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN
