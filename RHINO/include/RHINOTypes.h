#pragma once

#include <cstdint>
#include <cstdlib>

namespace RHINO {
    class Buffer;
    class Texture2D;
    class DescriptorHeap;

    class RTPSO;
    class ComputePSO;
    class CommandList;

    enum class TextureFormat {

    };

    enum class ResourceHeapType {
        Default,
        Upload,
        Readback,
    };

    enum class ResourceUsage : size_t
    {
        None = 0x0,
        VertexBuffer = 0x1,
        IndexBuffer = 0x2,
        ConstantBuffer = 0x4,
        ShaderResource = 0x8,
        UnorderedAccess = 0x10,
        Indirect = 0x20,
        CopySource = 0x40,
        CopyDest = 0x80,
        ValidMask = 0xFF,
    };

    enum class DescriptorHeapType
    {
        SRV_CBV_UAV,
        RTV,
        DSV,
        Sampler,
        Count,
    };

    enum class DescriptorType {
        BufferSRV,
        BufferUAV,
        BufferCBV,
        Texture2DSRV,
        Texture2DUAV,
        Texture3DSRV,
        Texture3DUAV,
        Sampler,
        Count,
    };

    enum class DescriptorRangeType {
        CBV,
        SRV,
        UAV,
        Sampler
    };

    enum class ResourceType {
        Buffer,
        Texture2D,
        Texture3D,
        Count,
    };

    struct DescriptorRangeDesc {
        DescriptorRangeType rangeType = DescriptorRangeType::CBV;
        size_t descriptorsCount = 0;
        size_t baseRegisterSlot = 0;
    };

    struct DescriptorSpaceDesc {
        size_t space = 0;
        size_t offsetInDescriptorsFromTableStart = 0;
        size_t rangeDescCount = 0;
        const DescriptorRangeDesc* rangeDescs = nullptr;
    };

    struct ShaderModule {
        size_t bytecodeSize;
        const uint8_t* bytecode;
        const char* entrypoint;
    };

    struct RTPSODesc {
        const char* debugName;
    };

    struct ComputePSODesc {
        size_t spacesCount = 0;
        const DescriptorSpaceDesc* spacesDescs = nullptr;
        ShaderModule CS = {};
        const char* debugName = "UnnamedComputePSO";
    };

    struct DispatchDesc {
        size_t dimensionsX;
        size_t dimensionsY;
        size_t dimensionsZ;
    };

    class CommandList {
    public:
        virtual ~CommandList() noexcept = default;
    public:
        virtual void Dispatch(const DispatchDesc& desc) noexcept = 0;
        virtual void Draw() noexcept = 0;
        virtual void SetComputePSO(ComputePSO* pso) noexcept = 0;
        virtual void SetRTPSO(RTPSO* pso) noexcept = 0;
        virtual void SetHeap(DescriptorHeap* heap) noexcept = 0;
    };

    struct WriteBufferSRVDesc {
        Buffer* buffer = nullptr;
        size_t bufferOffset = 0;
        size_t offsetInHeap = 0;
    };

    struct WriteTexture2DSRVDesc {
        Texture2D* texture = nullptr;
        size_t offsetInHeap = 0;
    };

    struct WriteTexture3DSRVDesc {
        Texture2D* texture = nullptr;
        size_t offsetInHeap = 0;
    };

    class DescriptorHeap {
    public:
        virtual ~DescriptorHeap() noexcept = default;
    public:
        virtual void WriteSRV(const WriteBufferSRVDesc& desc) noexcept = 0;
        virtual void WriteUAV(const WriteBufferSRVDesc& desc) noexcept = 0;
        virtual void WriteCBV(const WriteBufferSRVDesc& desc) noexcept = 0;

        virtual void WriteSRV(const WriteTexture2DSRVDesc& desc) noexcept = 0;
        virtual void WriteUAV(const WriteTexture2DSRVDesc& desc) noexcept = 0;

        virtual void WriteSRV(const WriteTexture3DSRVDesc& desc) noexcept = 0;
        virtual void WriteUAV(const WriteTexture3DSRVDesc& desc) noexcept = 0;
    };
}
