out vec4 frag_color;
in vec2 vUV;

void main() {
    // https://stackoverflow.com/questions/4694608/glsl-checkerboard-pattern
    float total = floor(floor(vUV.x * 10.0) + floor(10.0 * vUV.y));
    bool isEven = mod(total, 2.0) == 0.0;
    vec4 col1 = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 col2 = vec4(1.0, 1.0, 1.0, 1.0);
    frag_color = (isEven) ? col1 : col2;
}
