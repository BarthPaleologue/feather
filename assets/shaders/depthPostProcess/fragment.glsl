out vec4 fragColor;
in vec2 vUV;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

uniform vec2 screenResolution;

void main()
{
    vec3 screenColor = texture(screenTexture, vUV).rgb;
    float depth = texture(depthTexture, vUV).r;

    vec3 color = 1.0 - screenColor;//vec3(depth, depth, vUV.x);
    fragColor = vec4(color, 1.0);
}