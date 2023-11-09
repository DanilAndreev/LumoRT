#pragma once

#ifdef ENABLE_API_VULKAN

#include "VulkanBackendTypes.h"

namespace RHINO::APIVulkan {
    class VulkanCommandList : public CommandList {
    public:
        void Dispatch(const DispatchDesc& desc) noexcept final;
        void Draw() noexcept final;
        void SetComputePSO(ComputePSO* pso) noexcept final;
        void SetRTPSO(RTPSO* pso) noexcept final;
        void SetHeap(DescriptorHeap* heap) noexcept final;

    public:
        VkCommandBuffer cmd = VK_NULL_HANDLE;

    private:
        VulkanComputePSO* m_ComputePSO = nullptr;
    };
}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN
