#version 400

out vec4 frag_color;
in vec3 fColor;

varying vec3 vPosition;
varying vec3 vPositionW;
varying vec3 vNormal;
varying vec2 vUV;

uniform vec3 lightPosition;
uniform sampler2D texture;

void main() {
  vec3 texColor = texture2D(texture, vUV).rgb;
  vec3 color = texColor;//vec3(vUV, 0.0);
  float ndl = 1.0;//max(dot(vNormal, normalize(lightPosition - vPositionW)), 0.0);
  frag_color = vec4(color * ndl, 1.0);
}
