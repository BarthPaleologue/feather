out vec4 fragColor;

uniform float exposure;
uniform float gamma;
uniform float contrast;
uniform float saturation;
uniform float brightness;

uniform sampler2D screenTexture;

in vec2 vUV;

vec3 saturate(vec3 color) {
    if (color.r > 1.0) {
        color.r = 1.0;
    }
    if (color.g > 1.0) {
        color.g = 1.0;
    }
    if (color.b > 1.0) {
        color.b = 1.0;
    }
    if (color.r < 0.0) {
        color.r = 0.0;
    }
    if (color.g < 0.0) {
        color.g = 0.0;
    }
    if (color.b < 0.0) {
        color.b = 0.0;
    }
    return color;
}

void main() {
    vec4 screenColor = texture(screenTexture, vUV);
    vec3 color = screenColor.rgb;
    float alpha = screenColor.a;

    color *= exposure;
    color = saturate(color);

    color = (color - 0.5) * contrast + 0.5 + brightness;
    color = saturate(color);

    vec3 grayscale = vec3(0.299, 0.587, 0.114) * color;
    color = mix(grayscale, color, saturation);
    color = saturate(color);

    color = pow(color, vec3(gamma));

    fragColor = vec4(color, alpha);
}