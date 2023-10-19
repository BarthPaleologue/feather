out vec4 fragColor;
in vec2 vUV;

uniform sampler2D screenTexture;

uniform vec2 screenResolution;

void main()
{
    vec4 screenColor = texture(screenTexture, vUV);
    vec3 color = 1.0 - screenColor.rgb;
    fragColor = vec4(color, screenColor.a);
}