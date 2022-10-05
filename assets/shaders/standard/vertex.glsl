layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=1) in vec3 vColor;
layout(location=3) in vec2 uv;
out vec3 fColor;

varying vec3 vPosition;
varying vec3 vPositionW;
varying vec3 vNormal;
varying vec3 vNormalW;
varying vec2 vUV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform mat4 normalMatrix;

void main() {
    fColor = vColor;
    vPosition = position;
    vPositionW = vec3(world * vec4(position, 1.0));
    vNormal = normal;
    vNormalW = mat3(normalMatrix) * normal;
    vUV = uv;

    gl_Position = projection * view * world * vec4(position, 1.0);
}
