#pragma once

#ifdef ENABLE_API_VULKAN

namespace RHINO::APIVulkan {
    class VulkanDescriptorHeap;

    class VulkanBuffer : public Buffer {
    public:
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceAddress deviceAddress = 0;
    };

    class VulkanTexture2D : public Texture2D {
    public:
        VkImage texture = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        VkImageLayout layout = VK_IMAGE_LAYOUT_GENERAL;
    };

    class VulkanRTPSO : public RTPSO {
    public:
    };

    class VulkanComputePSO : public ComputePSO {
    public:
        VkPipeline PSO;
        VkPipelineLayout layout;
    };
}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN