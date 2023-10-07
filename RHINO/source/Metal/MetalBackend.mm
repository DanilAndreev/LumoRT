#import "MetalBackendTypes.h"

#ifdef ENABLE_API_METAL

namespace RHINO::APIMetal {

    class MetalBackend : public RHINOInterface {
    public:
        MetalBackend() noexcept {}

    public:
        RTPSO* CompileRTPSO() noexcept final {
            return nullptr;
        }

        void ReleaseRTPSO(RTPSO* pso) noexcept final {
            delete pso;
        }

        ComputePSO* CompileComputePSO() noexcept final {
            return nullptr;
        }

        void ReleaseComputePSO(ComputePSO* pso) noexcept final {
        }

    public:
        Buffer* CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept final {
        }

        void ReleaseBuffer(Buffer* buffer) noexcept final {
            delete buffer;
        }

        Buffer* CreateTexture2D() noexcept final {
        }

        void ReleaseTexture2D(Texture2D* texture) noexcept final {
            delete texture;
        }

        DescriptorHeap* CreateDescriptorHeap() noexcept final {
            return nullptr;
        }
        void ReleaseDescriptorHeap(DescriptorHeap* heap) noexcept final {
            delete heap;
        }

    public:
        void DispatchCompute() noexcept final {
        }
        void DispatchComputeIndirect() noexcept final {
        }
        void TraceRays() noexcept final {
        }
    };

    RHINOInterface* AllocateMetalBackend() noexcept {
        return new MetalBackend{};
    }
}// namespace RHINO::APID3D12

#endif // ENABLE_API_METAL
