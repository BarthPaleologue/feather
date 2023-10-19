out vec4 fragColor;
in vec2 vUV;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

uniform vec2 screenResolution;

void main()
{
    float depth = texture(depthTexture, vUV).r;
    vec3 color = vec3(depth);
    fragColor = vec4(color, 1.0);
}