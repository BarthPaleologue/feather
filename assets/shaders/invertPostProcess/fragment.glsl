out vec4 fragColor;
in vec2 vUV;

uniform sampler2D screenTexture;

uniform vec2 screenResolution;

void main()
{
    vec4 screenColor = texture2D(screenTexture, vUV);
    fragColor = vec4(vec3(1.0f) - screenColor.rgb, screenColor.a);
}