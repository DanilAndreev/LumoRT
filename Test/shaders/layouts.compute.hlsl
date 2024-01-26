RWStructuredBuffer<int> DestUAV1 : register(u0, space0);
RWStructuredBuffer<int> DestUAV2 : register(u1, space0);
RWStructuredBuffer<int> DestUAV3 : register(u3, space0);

[numthreads(64, 1, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    DestUAV1[DTid.x] = int(DTid.x);
    DestUAV2[DTid.x] = 1000 - int(DTid.x);
    DestUAV3[DTid.x] = -int(DTid.x);
}
