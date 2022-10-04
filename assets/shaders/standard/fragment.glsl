out vec4 frag_color;
in vec3 fColor;

varying vec3 vPosition;
varying vec3 vPositionW;
varying vec3 vNormal;
varying vec2 vUV;

uniform vec3 lightPosition;
uniform vec3 emissiveColor;

#ifdef DIFFUSE_TEXTURE
uniform sampler2D diffuseTexture;
#endif

void main() {
    vec3 color = vec3(0.0);
    vec3 diffuseColor = vec3(0.0);

    #ifdef DIFFUSE_TEXTURE
    diffuseColor += texture2D(diffuseTexture, vUV).rgb;
    #endif

    float ndl = max(dot(vNormal, normalize(lightPosition - vPositionW)), 0.0);

    color += diffuseColor * ndl;

    color += emissiveColor;

    frag_color = vec4(color, 1.0);
}
