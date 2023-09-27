layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=1) in vec3 vColor;
layout(location=3) in vec2 uv;

out vec2 vUV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

void main() {
    vUV = uv;
    gl_Position = projection * view * world * vec4(position, 1.0);
}
