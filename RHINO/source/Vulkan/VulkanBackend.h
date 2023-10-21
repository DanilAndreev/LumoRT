#pragma once

#ifdef ENABLE_API_VULKAN

#include "VulkanBackendTypes.h"

namespace RHINO::APIVulkan {
    class VulkanDescriptorHeap;
    class VulkanCommandList;

    class VulkanBackend final : public RHINOInterface {
    public:
        explicit VulkanBackend() noexcept = default;

    public:
        void Initialize() noexcept final;
        void Release() noexcept final;

    public:
        RTPSO* CompileRTPSO(const RTPSODesc& desc) noexcept final;
        void ReleaseRTPSO(RTPSO* pso) noexcept final;
        ComputePSO* CompileComputePSO(const ComputePSODesc& desc) noexcept final;
        void ReleaseComputePSO(ComputePSO* pso) noexcept final;

        Buffer* CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept final;
        void ReleaseBuffer(Buffer* buffer) noexcept final;
        Texture2D* CreateTexture2D() noexcept final;
        void ReleaseTexture2D(Texture2D* texture) noexcept final;
        DescriptorHeap* CreateDescriptorHeap(DescriptorHeapType type, size_t descriptorsCount, const char* name) noexcept final;
        void ReleaseDescriptorHeap(DescriptorHeap* heap) noexcept final;

        CommandList* AllocateCommandList(const char* name) noexcept final;
        void ReleaseCommandList(CommandList* commandList) noexcept final;

    private:
        uint32_t SelectMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) noexcept;
        void SelectQueues(VkDeviceQueueCreateInfo queueInfos[3], uint32_t* infosCount) noexcept;
        size_t CalculateDescriptorHandleIncrementSize(DescriptorHeapType heapType) noexcept;
    private:
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice;
        VkAllocationCallbacks* m_Alloc = nullptr;
    };
}// namespace RHINO::APIVulkan

#endif//  ENABLE_API_VULKAN