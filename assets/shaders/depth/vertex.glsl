layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=1) in vec3 color;
layout(location=3) in vec2 uv;

uniform mat4 projectionView;
uniform mat4 world;
uniform mat4 normalMatrix;

void main() {
    gl_Position = projectionView * world * vec4(position, 1.0);
}
