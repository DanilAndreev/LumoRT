#pragma once

#include "RHINOTypes.h"

namespace RHINO {
    enum class BackendAPI {
        D3D12,
        Metal
    };

    class RHINOInterface {
    public:
        RHINOInterface() = default;
        RHINOInterface(const RHINOInterface&) = delete;
        RHINOInterface(const RHINOInterface&&) = delete;
        virtual ~RHINOInterface() noexcept = default;
    public:
        virtual void Initialize() noexcept = 0;
        virtual void Release() noexcept = 0;

    public:
        // PSO MANAGEMENT
        virtual RTPSO* CompileRTPSO(const RTPSODesc& desc) noexcept = 0;
        virtual void ReleaseRTPSO(RTPSO* pso) noexcept = 0;
        virtual ComputePSO* CompileComputePSO(const ComputePSODesc& desc) noexcept = 0;
        virtual void ReleaseComputePSO(ComputePSO* pso) noexcept = 0;

    public:
        // RESOURCE MANAGEMENT
        virtual Buffer* CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept = 0;
        virtual void ReleaseBuffer(Buffer* buffer) noexcept = 0;
        virtual Texture2D* CreateTexture2D() noexcept = 0;
        virtual void ReleaseTexture2D(Texture2D* texture) noexcept = 0;

        virtual DescriptorHeap* CreateDescriptorHeap(DescriptorHeapType type, size_t descriptorsCount, const char* name) noexcept = 0;
        virtual void ReleaseDescriptorHeap(DescriptorHeap* heap) noexcept = 0;

        virtual CommandList* AllocateCommandList(const char* name = "") noexcept = 0;
        virtual void ReleaseCommandList(CommandList* commandList) noexcept = 0;
    public:
        // GPU JOB SUBMISSION
        virtual void DispatchCompute() noexcept = 0;
        virtual void DispatchComputeIndirect() noexcept = 0;
        virtual void TraceRays() noexcept = 0;
    };

    RHINOInterface* CreateRHINO(BackendAPI backendApi) noexcept;
}// namespace RHINO
