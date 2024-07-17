RWTexture2D<float> OutImage : register(u0, space0);

float Parabola(float x) {
    return x * x;
}

[numthreads(64, 64, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {

    float y = Parabola(float(DTid.x));
    if (uint(y) == DTid.y) {
        OutImage[DTid.xy] = 1.0f;
    } else {
        OutImage[DTid.xy] = 0.1f;
    }
}
