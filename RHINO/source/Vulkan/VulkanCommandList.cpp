#ifdef ENABLE_API_VULKAN

#include "VulkanCommandList.h"
#include "VulkanAPI.h"
#include "VulkanDescriptorHeap.h"

namespace RHINO::APIVulkan {
    void VulkanCommandList::CopyBuffer(Buffer* src, Buffer* dst, size_t srcOffset, size_t dstOffset, size_t size) noexcept {
        auto* vulkanSrc = static_cast<VulkanBuffer*>(src);
        auto* vulkanDst = static_cast<VulkanBuffer*>(dst);
        VkBufferCopy region{};
        region.size = size;
        region.srcOffset = srcOffset;
        region.dstOffset = dstOffset;
        vkCmdCopyBuffer(cmd, vulkanSrc->buffer, vulkanDst->buffer, 1, &region);
    }

    void VulkanCommandList::Dispatch(const DispatchDesc& desc) noexcept {
        vkCmdDispatch(cmd, desc.dimensionsX, desc.dimensionsY, desc.dimensionsZ);
    }

    void VulkanCommandList::Draw() noexcept {
    }

    void VulkanCommandList::SetComputePSO(ComputePSO* pso) noexcept {
        auto* vulkanPSO = static_cast<VulkanComputePSO*>(pso);
        for (auto [space, spaceInfo] : vulkanPSO->heapOffsetsBySpaces) {
            VkDescriptorSet heap = spaceInfo.first == DescriptorRangeType::Sampler ? m_SamplerHeap : m_CBVSRVUAVHeap;
            uint32_t offset = spaceInfo.second;
            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, vulkanPSO->layout, space, 1, &heap,1, &offset);
        }
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, vulkanPSO->PSO);
    }

    void VulkanCommandList::SetRTPSO(RTPSO* pso) noexcept {

    }

    void VulkanCommandList::SetHeap(DescriptorHeap* CBVSRVUAVHeap, DescriptorHeap* SamplerHeap) noexcept {
        auto* vulkanCBVSRVUAVHeap = static_cast<VulkanDescriptorHeap*>(CBVSRVUAVHeap);
        m_CBVSRVUAVHeap = vulkanCBVSRVUAVHeap->heap;
        if (SamplerHeap) {
            auto* vulkanSamplerHeap = static_cast<VulkanDescriptorHeap*>(SamplerHeap);
            m_SamplerHeap = vulkanSamplerHeap->heap;
        }
    }
}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN
