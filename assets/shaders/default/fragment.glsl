out vec4 frag_color;

in vec3 vColor;
in vec3 vPosition;
in vec3 vPositionW;
in vec3 vNormal;
in vec3 vNormalW;
in vec2 vUV;

void main() {
    vec3 color = vec3(0.0);

    // 10x10 checkerboard
    if (mod(floor(vPosition.x * 10.0) + floor(vPosition.y * 10.0) + floor(vPosition.z * 10.0), 2.0) == 0.0) {
        color = vec3(1.0);
    }

    frag_color = vec4(color, 1.0);
}
