#pragma once

#include "Math3DTypes.h"

namespace Math3D {
    Float4x4 operator*(const Float4x4& lh, const Float4x4& rh) noexcept;
    Float4 operator*(const Float4x4& lh, const Float4& rh) noexcept;

    Float4x4 PerspectiveWithFOVY(float FOVY, float aspect, float near, float far) noexcept;
    Float4x4 PerspectiveWithFOVY(float FOVY, float width, float height, float near, float far) noexcept;

    Float4x4 LookAt(const Float3 &eyePos, const Float3 &targetPos, const Float3 &up) noexcept;

    Float4x4 Transpose(const Float4x4& v) noexcept;
    Float4x4 Inverse(const Float4x4& v) noexcept;
}// namespace Math3D
