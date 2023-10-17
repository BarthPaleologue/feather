out vec4 frag_color;
in vec3 fColor;

in vec3 vPosition;
in vec3 vPositionW;
in vec3 vNormal;
in vec3 vNormalW;
in vec2 vUV;

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

uniform sampler2D diffuseTexture;
uniform sampler2D ambientTexture;

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

    frag_color = vec4(color, 1.0);
}
