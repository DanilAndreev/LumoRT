#pragma once

#include "Math3DHelpers.h"

namespace Math3D {
    IFDX(using namespace DirectX);

    union Float1 {
        float values[1];
        IFAPPLE(simd::float1 simd);
        DirectX::XMFLOAT3 simd;
    };

    union Float2 {
        float values[2];
        IFAPPLE(simd::float4 simd);
        IFDX(XMFLOAT2 simd);
    };

    union Float3 {
        float values[3];
        IFAPPLE(simd::float3 simd);
        IFDX(XMFLOAT3 simd);
    };

    union Float4 {
        float values[4];
        IFAPPLE(simd::float4 simd);
        IFDX(XMFLOAT4 simd);

        inline operator Float3() const noexcept { return Float3{values[0], values[1], values[2]}; }
        inline float& operator[](size_t index) noexcept { return values[index]; }
        inline const float& operator[](size_t index) const noexcept { return values[index]; }
    };

    union Float4x4 {
        Float4 columns[4];
        float values[16];
        IFAPPLE(simd::float4x4 simd);
        IFDX(XMMATRIX simd);

        inline Float4& operator[](size_t index) noexcept { return columns[index]; }
        inline const Float4& operator[](size_t index) const noexcept { return columns[index]; }
    };

    const Float4x4 Ident = Float4x4{
            Float4{1.0f, 0.0f, 0.0f, 0.0f},
            Float4{0.0f, 1.0f, 0.0f, 0.0f},
            Float4{0.0f, 0.0f, 1.0f, 0.0f},
            Float4{0.0f, 0.0f, 0.0f, 1.0f},
    };
}// namespace Math3D