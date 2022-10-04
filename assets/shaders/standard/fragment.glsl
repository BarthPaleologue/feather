out vec4 frag_color;
in vec3 fColor;

varying vec3 vPosition;
varying vec3 vPositionW;
varying vec3 vNormal;
varying vec2 vUV;

uniform vec3 lightPosition;

uniform vec3 diffuseColor;
uniform vec3 emissiveColor;
uniform vec3 alphaColor;

uniform sampler2D diffuseTexture;
uniform sampler2D emissiveTexture;

void main() {
    vec3 color = vec3(0.0);
    vec3 diffuseColor = diffuseColor;
    #ifdef DIFFUSE_TEXTURE
    diffuseColor = texture2D(diffuseTexture, vUV).rgb;
    #endif

    #ifdef ALPHA_COLOR
    if(diffuseColor == alphaColor) discard;
    #endif

    float ndl = max(dot(vNormal, normalize(lightPosition - vPositionW)), 0.0);

    color += diffuseColor * ndl;

    vec3 emissiveColor = emissiveColor;
    #ifdef EMISSIVE_TEXTURE
    emissiveColor = texture2D(emissiveTexture, vUV).rgb;
    #endif

    #ifdef ALPHA_COLOR
    if(emissiveColor == alphaColor) discard;
    #endif

    color += emissiveColor;

    frag_color = vec4(color, 1.0);
}
