#include "fractalTypes.hlsli"

Texture2D SrcImage : register(s1, space0);
RWTexture2D<float> OutImage : register(u3, space0);
SamplerState MeshTextureSampler : register(s0, space1);

[numthreads(EXAMPLE_FRACTAL_BLIT_THREADGROUP_X, EXAMPLE_FRACTAL_BLIT_THREADGROUP_Y, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    float2 uv = float2(DTid.xy) / float2(512.0f, 512.0f);
    OutImage[DTid.xy] = SrcImage.SampleLevel(MeshTextureSampler, uv, 0);
}
