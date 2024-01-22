#ifdef ENABLE_API_VULKAN

#include "VulkanDescriptorHeap.h"
#include "VulkanAPI.h"

namespace RHINO::APIVulkan {
    void VulkanDescriptorHeap::WriteSRV(const RHINO::WriteBufferSRVDesc& desc) noexcept {
        auto* vulkanBuffer = static_cast<VulkanBuffer*>(desc.buffer);
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = vulkanBuffer->buffer;
        bufferInfo.offset = desc.bufferOffset;
        bufferInfo.range = VK_WHOLE_SIZE;

        VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        write.descriptorCount = 1;
        write.dstBinding = desc.offsetInHeap;
        write.dstArrayElement = 0;
        write.dstSet = heap;
        write.pBufferInfo = &bufferInfo;
    }
    void VulkanDescriptorHeap::WriteUAV(const WriteBufferSRVDesc& desc) noexcept {
        auto* vulkanBuffer = static_cast<VulkanBuffer*>(desc.buffer);
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = vulkanBuffer->buffer;
        bufferInfo.offset = desc.bufferOffset;
        bufferInfo.range = VK_WHOLE_SIZE;

        VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        write.descriptorCount = 1;
        write.dstBinding = desc.offsetInHeap;
        write.dstArrayElement = 0;
        write.dstSet = heap;
        write.pBufferInfo = &bufferInfo;
    }
    void VulkanDescriptorHeap::WriteCBV(const WriteBufferSRVDesc& desc) noexcept {
        auto* vulkanBuffer = static_cast<VulkanBuffer*>(desc.buffer);
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = vulkanBuffer->buffer;
        bufferInfo.offset = desc.bufferOffset;
        bufferInfo.range = VK_WHOLE_SIZE;

        VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.descriptorCount = 1;
        write.dstBinding = desc.offsetInHeap;
        write.dstArrayElement = 0;
        write.dstSet = heap;
        write.pBufferInfo = &bufferInfo;
    }
    void VulkanDescriptorHeap::WriteSRV(const WriteTexture2DSRVDesc& desc) noexcept {
        auto* vulkanTexture2D = static_cast<VulkanTexture2D*>(desc.texture);
        VkDescriptorImageInfo imageInfo{};
        imageInfo.sampler = VK_NULL_HANDLE;
        imageInfo.imageLayout = vulkanTexture2D->layout;
        imageInfo.imageView = VK_NULL_HANDLE; //TODO: finish

        VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        write.descriptorCount = 1;
        write.dstBinding = desc.offsetInHeap;
        write.dstArrayElement = 0;
        write.dstSet = heap;
        write.pImageInfo = &imageInfo;
    }
    void VulkanDescriptorHeap::WriteUAV(const WriteTexture2DSRVDesc& desc) noexcept {
        auto* vulkanTexture2D = static_cast<VulkanTexture2D*>(desc.texture);
        VkDescriptorImageInfo imageInfo{};
        imageInfo.sampler = VK_NULL_HANDLE;
        imageInfo.imageLayout = vulkanTexture2D->layout;
        imageInfo.imageView = VK_NULL_HANDLE; //TODO: finish

        VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        write.descriptorCount = 1;
        write.dstBinding = desc.offsetInHeap;
        write.dstArrayElement = 0;
        write.dstSet = heap;
        write.pImageInfo = &imageInfo;
    }
    void VulkanDescriptorHeap::WriteSRV(const WriteTexture3DSRVDesc& desc) noexcept {
    }
    void VulkanDescriptorHeap::WriteUAV(const WriteTexture3DSRVDesc& desc) noexcept {
    }
}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN
