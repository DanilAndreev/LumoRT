[numthreads(256,1,1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    StructuredBuffer<uint> src = ResourceDescriptorHeap[0];
    RWStructuredBuffer<uint> dst = ResourceDescriptorHeap[1];
    dst[DTid.x] = src[DTid.x];
}
