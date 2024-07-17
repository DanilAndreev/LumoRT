#include <fractalTypes.hlsli>

ConstantBuffer<FractalSettings> Constants : register(b0, space0);
RWTexture2D<float> OutImage : register(u2, space0);

float Parabola(float x) {
    return x * x;
}

[numthreads(32, 32, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    float xL = float(DTid.x) - 0.5f;
    float xR = float(DTid.x) + 0.5f;
    float pixelY = float(DTid.y);

    float yL = Parabola((xL + Constants.moveX) * Constants.leanX) / Constants.leanY + Constants.moveY;
    float yR = Parabola((xR + Constants.moveX) * Constants.leanX) / Constants.leanY + Constants.moveY;

    if (pixelY - 0.5f < max(yL, yR) && pixelY + 0.5f > min(yL, yR)) {
        OutImage[DTid.xy] = 1.0f;
    } else {
        OutImage[DTid.xy] = 0.1f;
    }
}
