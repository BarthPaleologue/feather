out vec4 frag_color;
in vec2 vUV;

uniform sampler2D screenTexture;

void main() {
    vec4 screenColor = texture(screenTexture, vUV);
    vec3 color = vec3(1.0) - screenColor.rgb;
    frag_color = vec4(color, screenColor.a);
}
