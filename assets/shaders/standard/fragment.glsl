out vec4 frag_color;
in vec3 fColor;

varying vec3 vPosition;
varying vec3 vPositionW;
varying vec3 vNormal;
varying vec3 vNormalW;
varying vec2 vUV;

uniform mat4 world;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform vec3 alphaColor;

uniform sampler2D diffuseTexture;
uniform sampler2D ambientTexture;

void main() {
    vec3 color = vec3(0.0);
    vec3 diffuseColor = diffuseColor;
    #ifdef DIFFUSE_TEXTURE
    diffuseColor = texture2D(diffuseTexture, vUV).rgb;
    #endif

    #ifdef ALPHA_COLOR
    if(diffuseColor == alphaColor) discard;
    #endif

    float ndl = max(dot(vNormalW, normalize(lightPosition - vPositionW)), 0.0);

    color += diffuseColor * ndl;

    vec3 ambientColor = ambientColor;
    #ifdef AMBIENT_TEXTURE
    ambientColor = texture2D(ambientTexture, vUV).rgb;
    #endif

    #ifdef ALPHA_COLOR
    if(ambientColor == alphaColor) discard;
    #endif

    color += ambientColor;

    vec3 lightRayW = normalize(lightPosition - vPositionW);
    vec3 viewDirW = normalize(cameraPosition - vPositionW);
    vec3 angleW = normalize(viewDirW + lightRayW);
    float specComp = max(0., dot(normalize(vec3(world * vec4(vNormal, 0.0))), angleW));
    specComp = pow(specComp, max(1.0, 64.0));

    color += vec3(specComp);

    frag_color = vec4(color, 1.0);
}
