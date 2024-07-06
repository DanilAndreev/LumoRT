// RWTexture2D<float4> T[] : register(u40, space0);
RWStructuredBuffer<int> DestUAV12[2] : register(u0, space0);
// RWStructuredBuffer<int> DestUAV2 : register(u1, space0);
RWStructuredBuffer<int> DestUAV3 : register(u1, space1);

[numthreads(64, 1, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
//     T[0][DTid.xy] = float4(1, 1, 1, 1);
//     T[1][DTid.xy] = float4(0.5, 0.5, 0.5, 1);
//     T[2][DTid.xy] = float4(0.1, 0.5, 0.1, 1);
    DestUAV12[0][DTid.x] = int(DTid.x);
    DestUAV12[1][DTid.x] = 1000 - int(DTid.x);
    DestUAV3[DTid.x] = -int(DTid.x);
}
