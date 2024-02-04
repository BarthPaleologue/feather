out vec4 frag_color;

in vec3 vColor;
in vec3 vPosition;
in vec3 vPositionW;
in vec3 vNormal;
in vec3 vNormalW;
in vec2 vUV;

void main() {
    // pink
    vec3 color = vec3(1.0, 0.0, 1.0);

    frag_color = vec4(color, 1.0);
}
