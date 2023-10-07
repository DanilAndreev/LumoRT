#pragma once

#ifdef ENABLE_API_METAL

#import <Metal/Metal.h>

namespace RHINO::APIMetal {
    class MetalBuffer : public Buffer {
    public:
        id<MTLBuffer> buffer = nil;
    };

    class MetalTexture2D : public Texture2D {
    public:
        id<MTLTexture> texture;
    };

    class MetalDescriptorHeap : public DescriptorHeap {
    public:
        id<MTLBuffer> argBuf = nil;
        id<MTLArgumentEncoder> encoder = nil;
    };

    class MetalCommandList : public CommandList {
    public:
        id<MTLCommandBuffer> cmd = nil;
    };
}// namespace RHINO::APIMetal

#endif// ENABLE_API_METAL