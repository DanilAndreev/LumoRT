#include <fractalTypes.hlsli>

RWTexture2D<float> OutImage : register(u0, space0);
ConstantBuffer<FractalSettings> Constants : register(b1, space0);

float Parabola(float x) {
    return x * x;
}

[numthreads(32, 32, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    float xL = float(DTid.x) - 0.5;
    float xR = float(DTid.x) + 0.5;
    float pixelY = float(DTid.y);

    float yL = Parabola(xL * Constants.leanX + Constants.moveX) / Constants.leanY + Constants.moveY;
    float yR = Parabola(xR * Constants.leanX + Constants.moveX) / Constants.leanY + Constants.moveY;
    if (pixelY < max(yL, yR) && pixelY > min(yL, yR)) {
        OutImage[DTid.xy] = 1.0f;
    } else {
        OutImage[DTid.xy] = 0.1f;
    }
}
