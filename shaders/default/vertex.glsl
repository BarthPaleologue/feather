#version 400

layout(location=0) in vec3 position;
layout(location=1) in vec3 vColor;
out vec3 fColor;

uniform mat4 MVP;

void main() {
    fColor = vColor;
    gl_Position = MVP * vec4(position, 1.0);
}
