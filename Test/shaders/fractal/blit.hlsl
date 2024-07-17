Texture2D<float> SrcImage : register(s1, space0);
RWTexture2D<float> OutImage : register(u3, space0);

[numthreads(32, 32, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    OutImage[DTid.xy] = SrcImage[DTid.xy];
}