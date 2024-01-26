#pragma once

#include <string>

namespace RHINO::DebugLayer {
    class DebugLayer final : public RHINOInterface {
    public:
        explicit DebugLayer(RHINOInterface* wrapped) noexcept;

    public:
        void Initialize() noexcept final;
        void Release() noexcept final;
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
        void SubmitCommandList(CommandList* cmd) noexcept final;
    private:
        static void DB(const std::string& text) noexcept;

        // Enum to String
        static const char* EtoS(ResourceUsage usage) noexcept;

    private:
        RHINOInterface* m_Wrapped = nullptr;
    };
}// namespace RHINO::DebugLayer
