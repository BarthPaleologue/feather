layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=1) in vec3 vColor;
layout(location=3) in vec2 uv;
out vec3 fColor;

out vec3 vPosition;
out vec3 vPositionW;
out vec3 vNormal;
out vec3 vNormalW;
out vec2 vUV;

uniform mat4 projectionView;
uniform mat4 world;
uniform mat4 normalMatrix;

void main() {
    fColor = vColor;
    vPosition = position;
    vPositionW = vec3(world * vec4(position, 1.0));
    vNormal = normal;
    vNormalW = normalize(vec3(mat4(normalMatrix) * vec4(normal, 1.0)));
    vUV = uv;

    gl_Position = projectionView * world * vec4(position, 1.0);
}
