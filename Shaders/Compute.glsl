#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D out_tex;


void main() {
    // get position to read/write data from
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    // get value stored in the image
    vec4 in_val = imageLoad(out_tex, pos);
    // store new value in image
    imageStore(out_tex, pos, in_val + vec4(0.001, 0, 0, 0));
}
