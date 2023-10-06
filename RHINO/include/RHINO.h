#pragma once

#include "RHINOTypes.h"

namespace RHINO {
    class RHINOInterface {
    public:
        RHINOInterface(const RHINOInterface&&) = delete;
        virtual ~RHINOInterface() noexcept = default;
    public:
        virtual Buffer* CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept = 0;
        virtual Buffer* CreateTexture2D() noexcept = 0;
        virtual RTPSO* CompileRTPSO() noexcept = 0;
    };
}
