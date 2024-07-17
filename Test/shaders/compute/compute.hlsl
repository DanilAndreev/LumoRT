RWStructuredBuffer<int> DestUAV12[] : register(u0, space0);
RWStructuredBuffer<int> DestUAV3 : register(u1, space1);

[numthreads(64, 1, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    DestUAV12[0][DTid.x] = int(DTid.x);
    DestUAV12[1][DTid.x] = 1000 - int(DTid.x);
    DestUAV3[DTid.x] = -int(DTid.x);
}
