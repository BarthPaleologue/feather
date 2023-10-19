precision highp float;

layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=1) in vec3 color;
layout(location=3) in vec2 uv;

out vec3 vColor;
out vec3 vPosition;
out vec3 vPositionW;
out vec3 vNormal;
out vec3 vNormalW;
out vec2 vUV;

uniform mat4 projectionView;
uniform mat4 world;
uniform mat4 normalMatrix;

#ifdef SHADOW_MAP
out vec4 vPositionShadow;
uniform mat4 lightSpaceMatrix;
#endif

void main() {
    vColor = color;
    vPosition = position;
    vPositionW = vec3(world * vec4(position, 1.0));
    vNormal = normal;
    vNormalW = normalize(vec3(mat4(normalMatrix) * vec4(normal, 1.0)));
    vUV = uv;

    #ifdef SHADOW_MAP
    vPositionShadow = lightSpaceMatrix * world * vec4(position, 1.0);
    #endif

    gl_Position = projectionView * world * vec4(position, 1.0);
}
