#version 460
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(std430, set = 0, binding = 0) buffer DestUAV1 {
    int a[];
} destUAV1;
layout(std430, set = 0, binding = 1) buffer DestUAV2 {
    int a[];
} destUAV2;
layout(std430, set = 1, binding = 1) buffer DestUAV3 {
    int a[];
} destUAV3;

void main(void) {
    uvec3 DTid = gl_GlobalInvocationID;
    destUAV1.a[DTid.x] = int(DTid.x);
    destUAV2.a[DTid.x] = 1000 - int(DTid.x);
    destUAV3.a[DTid.x] = -int(DTid.x);
}
