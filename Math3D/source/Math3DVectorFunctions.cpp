#include "Math3DVectorFunctions.h"

namespace Math3D {
    /// FLOAT3 OPS
    Float3 operator*(const Float3 &lh, float rh) noexcept {
        return Float3{lh.values[0] * rh,
                      lh.values[1] * rh,
                      lh.values[2] * rh};
    }

    Float3 operator/(const Float3 &lh, float rh) noexcept {
        return Float3{lh.values[0] / rh,
                      lh.values[1] / rh,
                      lh.values[2] / rh};
    }

    Float3 operator+(const Float3 &lh, float rh) noexcept {
        return Float3{lh.values[0] + rh,
                      lh.values[1] + rh,
                      lh.values[2] + rh};
    }

    Float3 operator-(const Float3 &lh, float rh) noexcept {
        return Float3{lh.values[0] - rh,
                      lh.values[1] - rh,
                      lh.values[2] - rh};
    }
    Float3 operator-(const Float3 &v) noexcept {
        return Float3{-v.values[0],
                      -v.values[1],
                      -v.values[2]};
    }


    Float3 operator+(const Float3 &lh, const Float3 &rh) noexcept {
        return Float3{lh.values[0] + rh.values[0],
                      lh.values[1] + rh.values[1],
                      lh.values[2] + rh.values[2]};
    }
    Float3 operator-(const Float3 &lh, const Float3 &rh) noexcept {
        return Float3{lh.values[0] - rh.values[0],
                      lh.values[1] - rh.values[1],
                      lh.values[2] - rh.values[2]};
    }

    /// FLOAT4 OPS
    Float4 operator*(const Float4 &lh, float rh) noexcept {
        return Float4{lh.values[0] * rh,
                      lh.values[1] * rh,
                      lh.values[2] * rh,
                      lh.values[3] * rh};
    }

    Float4 operator/(const Float4 &lh, float rh) noexcept {
        return Float4{lh.values[0] / rh,
                      lh.values[1] / rh,
                      lh.values[2] / rh,
                      lh.values[3] / rh};
    }

    Float4 operator+(const Float4 &lh, float rh) noexcept {
        return Float4{lh.values[0] + rh,
                      lh.values[1] + rh,
                      lh.values[2] + rh,
                      lh.values[3] + rh};
    }

    Float4 operator-(const Float4 &lh, float rh) noexcept {
        return Float4{lh.values[0] - rh,
                      lh.values[1] - rh,
                      lh.values[2] - rh,
                      lh.values[3] - rh};
    }

    Float4 operator-(const Float4 &v) noexcept {
        return Float4{-v.values[0],
                      -v.values[1],
                      -v.values[2],
                      -v.values[3]};
    }


    Float4 operator+(const Float4 &lh, const Float4 &rh) noexcept {
        return Float4{lh.values[0] + rh.values[0],
                      lh.values[1] + rh.values[1],
                      lh.values[2] + rh.values[2],
                      lh.values[3] + rh.values[3]};
    }
    Float4 operator-(const Float4 &lh, const Float4 &rh) noexcept {
        return Float4{lh.values[0] - rh.values[0],
                      lh.values[1] - rh.values[1],
                      lh.values[2] - rh.values[2],
                      lh.values[3] - rh.values[3]};
    }

    float Length(const Float3 &v) noexcept {
        return sqrt(v.values[0] * v.values[0] + v.values[1] * v.values[1] + v.values[2] * v.values[2]);
    }
    Float3 Normalize(Float3 v) noexcept {
        float mod = Length(v);
        v.values[0] /= mod;
        v.values[1] /= mod;
        v.values[2] /= mod;
        return v;
    }

    float Dot(const Float3 &lh, const Float3 &rh) noexcept {
        return lh.values[0] * rh.values[0] + lh.values[1] * rh.values[1] + lh.values[2] * rh.values[2];
    }

    Float3 Cross(const Float3 &lh, const Float3 &rh) noexcept {
        static Float3 i{1.0f, 0.0f, 0.0f};
        static Float3 j{0.0f, 1.0f, 0.0f};
        static Float3 k{0.0f, 0.0f, 1.0f};

        Float3 r1 = i * (lh.values[1] * rh.values[2] - lh.values[2] * rh.values[1]);
        Float3 r2 = j * (lh.values[0] * rh.values[2] - lh.values[2] * rh.values[0]);
        Float3 r3 = k * (lh.values[0] * rh.values[1] - lh.values[1] * rh.values[0]);
        return r1 - r2 + r3;
    }
}// namespace Math3D
