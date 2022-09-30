#version 400

layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=1) in vec3 vColor;
layout(location=3) in vec2 uv;
out vec3 fColor;

varying vec3 vPosition;
varying vec3 vPositionW;
varying vec3 vNormal;
varying vec2 vUV;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 worldPosition;

void main() {
    fColor = vColor;
    vPosition = position;
    vPositionW = position + worldPosition;
    vNormal = normal;
    vUV = uv;

    gl_Position = projection * view * vec4(vPositionW, 1.0);
}
