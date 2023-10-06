#pragma once

namespace RHINO {
    class Buffer;
    class Texture2D;
    class RTPSO;

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
}
