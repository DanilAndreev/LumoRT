#pragma once

#ifdef ENABLE_API_VULKAN

#include "VulkanBackendTypes.h"

namespace RHINO::APIVulkan {
    class VulkanCommandList : public CommandList {
    public:
        void Dispatch(const DispatchDesc& desc) noexcept final;
        void Draw() noexcept final;
        void SetHeap(DescriptorHeap* heap) noexcept final;

    private:
        VkCommandBuffer cmd = VK_NULL_HANDLE;
    };
}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN
