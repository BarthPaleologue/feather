#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D out_tex;

void main() {
    // get position to read/write data from
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    vec2 uv = vec2(0.0);
    uv.x = float(pos.x)/(gl_NumWorkGroups.x);
    uv.y = float(pos.y)/(gl_NumWorkGroups.y);
    // get value stored in the image
    float in_val = imageLoad(out_tex, pos).r;
    // store new value in image
    imageStore(out_tex, pos, vec4(uv.x, uv.y, 0.0, 1.0));
}