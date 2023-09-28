out vec4 fragColor;

in float exposure;
in float gamma;
in float contrast;
in float saturation;
in float brightness;

uniform sampler2D screenTexture;

in vec2 vUV;

void main() {
    vec3 color = texture2D(textureSampler, vUV).rgb;
    float alpha = texture2D(textureSampler, vUV).a;

    color *= exposure;
    color = clamp(color, 0.0, 1.0);

    color = (color - 0.5) * contrast + 0.5 + brightness;
    color = clamp(color, 0.0, 1.0);

    vec3 grayscale = vec3(0.299, 0.587, 0.114) * color;
    color = lerp(color, grayscale, saturation);
    color = clamp(color, 0.0, 1.0);

    color = pow(color, vec3(gamma));

    fragColor = vec4(color, alpha);
}