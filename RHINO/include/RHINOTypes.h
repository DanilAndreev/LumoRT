#pragma once

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
}
