Texture2D SrcImage : register(s1, space0);
RWTexture2D<float> OutImage : register(u3, space0);
SamplerState MeshTextureSampler : register(s0, space1);

[numthreads(32, 32, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    float2 uv = float2(DTid.xy) / float2(64.0f, 64.0f);
    OutImage[DTid.xy] = SrcImage.SampleLevel(MeshTextureSampler, uv, 0);
}
