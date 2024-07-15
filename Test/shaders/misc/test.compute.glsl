#version 460
#extension GL_EXT_nonuniform_qualifier : enable
layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;
layout(std430, set = 0, binding = 0) readonly buffer BindlessBuf {
    int a;
} bindlessBuf[];
layout(std430, set = 0, binding = 0) readonly buffer BindlessBuf1 {
    uint a;
    float b;
} bindlessBuf1[];


layout(set = 0, binding = 0) uniform sampler2D texSRVs[];
uniform layout(set = 0, binding = 0, rgba8) writeonly image2D texUAVs[];

layout(std430, set = 1, binding = 0) buffer DestUAV {
    int total;
} destUAV;

void main(void) {
    uvec3 DTid = gl_GlobalInvocationID;
    destUAV.total += bindlessBuf[DTid.x].a;
    ivec2 pixelCoords = ivec2(0, 0);
    vec2 uv = vec2(0, 0);
    vec4 value = texture(texSRVs[DTid.x], uv);
    imageStore(texUAVs[DTid.x], pixelCoords, value);
}
