#pragma once

#ifdef ENABLE_API_METAL

namespace RHINO::APIMetal {
    class MetalBuffer : public Buffer {
    public:
    };

    class MetalTexture2D : public Texture2D {
    public:
    };
}// namespace RHINO::APIMetal

#endif // ENABLE_API_METAL