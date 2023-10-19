out vec4 frag_color;

in vec3 vColor;
in vec3 vPosition;
in vec3 vPositionW;
in vec3 vNormal;
in vec3 vNormalW;
in vec2 vUV;

#ifdef SHADOW_MAP
in vec4 vPositionShadow;
uniform vec3 lightDirection;
vec2 poissonDisk[4] = vec2[](
vec2(-0.94201624, -0.39906216),
vec2(0.94558609, -0.76890725),
vec2(-0.094184101, -0.92938870),
vec2(0.34495938, 0.29387760)
);
#endif

uniform mat4 world;

uniform int pointLightCount;
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int directionalLightCount;
struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

uniform vec3 cameraPosition;

uniform bool lightingEnabled;

uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform vec3 alphaColor;

layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D ambientTexture;
layout(binding = 2) uniform sampler2D shadowMap;

#ifdef SHADOW_MAP
float random(vec4 seed4) {
    float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float getShadowFactor(vec4 positionShadow) {
    vec3 shadowCoord = positionShadow.xyz / positionShadow.w;
    shadowCoord = shadowCoord * 0.5 + 0.5;

    float epsilon = max(0.05 * (1.0 - dot(vNormalW, lightDirection)), 0.005);

    float currentDepth = shadowCoord.z;

    float shadowMultiplier = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int i = 0; i < 4; i++) {
                vec2 offset = poissonDisk[i] / 700.0;
                vec2 texelCoord = shadowCoord.xy + vec2(x, y) * texelSize + offset;
                float texelDepth = texture(shadowMap, texelCoord).r;

                shadowMultiplier += currentDepth - epsilon > texelDepth ? 0.5 : 1.0;
            }
        }
    }
    shadowMultiplier /= 9 * 4;


    if (shadowCoord.z > 1.0) {
        shadowMultiplier = 1.0;
    }

    return shadowMultiplier;
}
#endif



void main() {
    vec3 color = vec3(0.0);

    if (lightingEnabled) {
        vec3 diffuseColor = diffuseColor;
        #ifdef DIFFUSE_TEXTURE
        diffuseColor = texture(diffuseTexture, vUV).rgb;
        #endif

        #ifdef ALPHA_COLOR
        if (diffuseColor == alphaColor) discard;
        #endif

        vec3 diffuseLightContributions = vec3(0.0);
        vec3 specularLightContributions = vec3(0.0);
        for (int i = 0; i < pointLightCount; i++) {
            float ndl = max(dot(vNormalW, normalize(pointLights[i].position - vPositionW)), 0.0);
            diffuseLightContributions += diffuseColor * pointLights[i].color * ndl * pointLights[i].intensity;

            vec3 lightRayW = normalize(pointLights[i].position - vPositionW);
            vec3 viewDirW = normalize(cameraPosition - vPositionW);
            vec3 angleW = normalize(viewDirW + lightRayW);
            float specComp = max(0., dot(normalize(vec3(world * vec4(vNormal, 0.0))), angleW));
            specularLightContributions += pow(specComp, 32.0) * pointLights[i].color * pointLights[i].intensity;
        }

        for (int i = 0; i < directionalLightCount; i++) {
            float ndl = max(dot(vNormalW, normalize(directionalLights[i].direction)), 0.0);
            diffuseLightContributions += diffuseColor * directionalLights[i].color * ndl * directionalLights[i].intensity;

            vec3 lightRayW = normalize(directionalLights[i].direction);
            vec3 viewDirW = normalize(cameraPosition - vPositionW);
            vec3 angleW = normalize(viewDirW + lightRayW);
            float specComp = max(0., dot(normalize(vec3(world * vec4(vNormal, 0.0))), angleW));
            specularLightContributions += pow(specComp, 32.0) * directionalLights[i].color * directionalLights[i].intensity;
        }

        color += diffuseColor * diffuseLightContributions + specularLightContributions;
    }

    vec3 ambientColor = ambientColor;
    #ifdef AMBIENT_TEXTURE
    ambientColor = texture(ambientTexture, vUV).rgb;
    #endif

    #ifdef ALPHA_COLOR
    if (ambientColor == alphaColor) discard;
    #endif

    color += ambientColor;


    #ifdef SHADOW_MAP
    //color = vec3(texture(shadowMap, vUV).r);
    color *= getShadowFactor(vPositionShadow);
    #endif

    frag_color = vec4(color, 1.0);
}
