#ifdef ENABLE_API_VULKAN

#include "VulkanCommandList.h"
#include "VulkanDescriptorHeap.h"

namespace RHINO::APIVulkan {
    void VulkanCommandList::Dispatch(const DispatchDesc& desc) noexcept {
        auto* vulkanComputePSO = static_cast<VulkanComputePSO*>(desc.pso);
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, vulkanComputePSO->PSO);
        vkCmdDispatch(cmd, desc.dimensionsX, desc.dimensionsY, desc.dimensionsZ);
    }

    void VulkanCommandList::Draw() noexcept {
    }

    void VulkanCommandList::SetComputePSO(ComputePSO* pso) noexcept {
        m_ComputePSO = static_cast<VulkanComputePSO*>(pso);
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePSO->PSO);
    }

    void VulkanCommandList::SetRTPSO(RTPSO* pso) noexcept {

    }

    void VulkanCommandList::SetHeap(DescriptorHeap* heap) noexcept {
        auto* vulkanDescriptorHeap = static_cast<VulkanDescriptorHeap*>(heap);
        VkDescriptorBufferBindingInfoEXT binding{VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT};
        binding.address = vulkanDescriptorHeap->heapGPUStartHandle;
        binding.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;
        vkCmdBindDescriptorBuffersEXT(cmd, 1, &binding);

        uint32_t bufferIndex = 0;
        VkDeviceSize offset = 0;
        vkCmdSetDescriptorBufferOffsetsEXT(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePSO->layout,
                                           0, 1, &bufferIndex, &offset);
    }
}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN
