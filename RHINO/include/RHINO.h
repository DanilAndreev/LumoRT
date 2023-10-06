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
        // PSO MANAGEMENT
        virtual RTPSO* CompileRTPSO() noexcept = 0;
        virtual void ReleaseRTPSO(RTPSO* pso) noexcept = 0;
        virtual ComputePSO* CompileComputePSO() noexcept = 0;
        virtual void ReleaseComputePSO(ComputePSO* pso) noexcept = 0;

    public:
        // RESOURCE MANAGEMENT
        virtual Buffer* CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept = 0;
        virtual void ReleaseBuffer(Buffer* buffer) noexcept = 0;
        virtual Buffer* CreateTexture2D() noexcept = 0;
        virtual void ReleaseTexture2D(Texture2D* texture) noexcept = 0;

        virtual DescriptorHeap* CreateDescriptorHeap() noexcept = 0;
        virtual void ReleaseDescriptorHeap(DescriptorHeap* heap) noexcept = 0;

    public:
        // GPU JOB SUBMISSION
        virtual void DispatchCompute() noexcept = 0;
        virtual void DispatchComputeIndirect() noexcept = 0;
        virtual void TraceRays() noexcept = 0;
    };

    RHINOInterface* CreateRHINO(BackendAPI backendApi) noexcept;
}// namespace RHINO
