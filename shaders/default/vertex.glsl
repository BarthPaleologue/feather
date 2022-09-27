#version 400

layout(location=0) in vec3 position;
layout(location=1) in vec3 vColor;
out vec3 fColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 worldPosition;

void main() {
    fColor = vColor;
    gl_Position = projection * view * vec4(position + worldPosition, 1.0);
}
