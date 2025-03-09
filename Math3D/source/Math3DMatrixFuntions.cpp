#include "Math3DEssentials.h"
#include "Math3DMatrixFunctions.h"
#include "Math3DVectorFunctions.h"

#include <cmath>

namespace Math3D {
    Float4x4 operator*(const Float4x4 &lh, const Float4x4 &rh) noexcept {
        Float4x4 res{};
        //TODO: add SSR AVX intrinsics
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    res[i][j] += lh[i][k] * rh[k][j];
                }
            }
        }
        return res;
    }

    Float4 operator*(const Float4x4& lh, const Float4& rh) noexcept {
        Float4 res{};
        res[0] = lh[0][0] * rh[0] + lh[0][1] * rh[1] + lh[0][2] * rh[2] + lh[0][3] * rh[3];
        res[1] = lh[1][0] * rh[0] + lh[1][1] * rh[1] + lh[1][2] * rh[2] + lh[1][3] * rh[3];
        res[2] = lh[2][0] * rh[0] + lh[2][1] * rh[1] + lh[2][2] * rh[2] + lh[2][3] * rh[3];
        res[3] = lh[3][0] * rh[0] + lh[3][1] * rh[1] + lh[3][2] * rh[2] + lh[3][3] * rh[3];
        return res;
    }

    Float4x4 PerspectiveWithFOVY(float FOVY, float aspect, float near, float far) noexcept {
        float angle = 0.5 * FOVY;
        float yScale = 1.0f / tanf(angle);
        float xScale = yScale / aspect;
        float zScale = far / (far - near);

        Float4 P;
        Float4 Q;
        Float4 R;
        Float4 S;

        P.values[0] = xScale;
        P.values[1] = 0.0f;
        P.values[2] = 0.0f;
        P.values[3] = 0.0f;

        Q.values[0] = 0.0f;
        Q.values[1] = yScale;
        Q.values[2] = 0.0f;
        Q.values[3] = 0.0f;

        R.values[0] = 0.0f;
        R.values[1] = 0.0f;
        R.values[2] = zScale;
        R.values[3] = 1.0f;

        S.values[0] = 0.0f;
        S.values[1] = 0.0f;
        S.values[2] = -near * zScale;
        S.values[3] = 0.0f;

        return Float4x4{P, Q, R, S};


        //        float fovYRadians = FOVY;
        //        float sy = 1.0f / tanf(fovYRadians * 0.5f);
        //        float sx = sy / aspect;
        //        float zRange = far - near;
        //        float sz = -(far + near) / zRange;
        //        float tz = -2 * far * near / zRange;
        //
        //        return Float4x4{Float4{sx, 0.0f, 0.0f, 0.0f},
        //                        Float4{0.0f, sy, 0.0f, 0.0f},
        //                        Float4{0.0f, 0.0f, sz, -1.0f},
        //                        Float4{0.0f, 0.0f, tz, 0.0f}};
    }

    Float4x4 PerspectiveWithFOVY(float FOVY, float width, float height, float near, float far) noexcept {
        float aspect = width / height;
        return PerspectiveWithFOVY(FOVY, aspect, near, far);
    }

    Float4x4 LookAt(const Float3 &eyePos, const Float3 &targetPos, const Float3 &up) noexcept {
        Float3 zAxis = Normalize(targetPos - eyePos);
        Float3 xAxis = Normalize(Cross(up, zAxis));
        Float3 yAxis = Cross(zAxis, xAxis);

        Float4 P{}, Q{}, R{}, S{};

        P.values[0] = xAxis.values[0];
        P.values[1] = yAxis.values[0];
        P.values[2] = zAxis.values[0];
        P.values[3] = 0.0f;

        Q.values[0] = xAxis.values[1];
        Q.values[1] = yAxis.values[1];
        Q.values[2] = zAxis.values[1];
        Q.values[3] = 0.0f;

        R.values[0] = xAxis.values[2];
        R.values[1] = yAxis.values[2];
        R.values[2] = zAxis.values[2];
        R.values[3] = 0.0f;

        S.values[0] = -Dot(xAxis, eyePos);
        S.values[1] = -Dot(yAxis, eyePos);
        S.values[2] = -Dot(zAxis, eyePos);
        S.values[3] = 1.0f;

        return Float4x4{P, Q, R, S};
    }

    Float4x4 Transpose(const Float4x4 &v) noexcept {
        Float4x4 result{};
        for (uint32_t i = 0; i < 4; ++i) {
            for (uint32_t j = 0; j < 4; ++j) {
                result.values[i * 4 + j] = v.values[j * 4 + i];
            }
        }
        return result;
    }

    Float4x4 Inverse(const Float4x4 &m) noexcept {
        float n11 = m.columns[0].values[0], n12 = m.columns[1].values[0], n13 = m.columns[2].values[0], n14 = m.columns[3].values[0];
        float n21 = m.columns[0].values[1], n22 = m.columns[1].values[1], n23 = m.columns[2].values[1], n24 = m.columns[3].values[1];
        float n31 = m.columns[0].values[2], n32 = m.columns[1].values[2], n33 = m.columns[2].values[2], n34 = m.columns[3].values[2];
        float n41 = m.columns[0].values[3], n42 = m.columns[1].values[3], n43 = m.columns[2].values[3], n44 = m.columns[3].values[3];

        float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
        float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
        float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
        float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

        float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
        float idet = 1.0f / det;

        Float4x4 ret{};
        ret.columns[0].values[0] = t11 * idet;
        ret.columns[0].values[1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
        ret.columns[0].values[2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
        ret.columns[0].values[3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

        ret.columns[1].values[0] = t12 * idet;
        ret.columns[1].values[1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
        ret.columns[1].values[2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
        ret.columns[1].values[3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

        ret.columns[2].values[0] = t13 * idet;
        ret.columns[2].values[1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
        ret.columns[2].values[2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
        ret.columns[2].values[3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

        ret.columns[3].values[0] = t14 * idet;
        ret.columns[3].values[1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
        ret.columns[3].values[2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
        ret.columns[3].values[3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;
        return ret;
    }
}// namespace Math3D
