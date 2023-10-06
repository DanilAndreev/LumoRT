#pragma once

#include "Math3DTypes.h"

namespace Math3D {
    /// FLOAT3 OPS
    Float3 operator*(const Float3 &lh, float rh) noexcept;
    Float3 operator/(const Float3 &lh, float rh) noexcept;
    Float3 operator+(const Float3 &lh, float rh) noexcept;
    Float3 operator-(const Float3 &lh, float rh) noexcept;
    Float3 operator-(const Float3 &v) noexcept;

    Float3 operator+(const Float3 &lh, const Float3 &rh) noexcept;
    Float3 operator-(const Float3 &lh, const Float3 &rh) noexcept;

    /// FLOAT4 OPS
    Float4 operator*(const Float4 &lh, float rh) noexcept;
    Float4 operator/(const Float4 &lh, float rh) noexcept;
    Float4 operator+(const Float4 &lh, float rh) noexcept;
    Float4 operator-(const Float4 &lh, float rh) noexcept;
    Float4 operator-(const Float4 &v) noexcept;

    Float4 operator+(const Float4 &lh, const Float4 &rh) noexcept;
    Float4 operator-(const Float4 &lh, const Float4 &rh) noexcept;

    /// OTHER

    float Length(const Float3 &v) noexcept;
    Float3 Normalize(Float3 v) noexcept;
    float Dot(const Float3 &lh, const Float3 &rh) noexcept;
    Float3 Cross(const Float3 &lh, const Float3 &rh) noexcept;
}// namespace Math3D
