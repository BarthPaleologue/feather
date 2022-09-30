#version 400

out vec4 frag_color;
in vec3 fColor;

varying vec3 vPosition;
varying vec3 vPositionW;
varying vec3 vNormal;
varying vec2 vUV;

uniform vec3 lightPosition;

void main() {
  vec3 color = vec3(vUV, 0.0);
  float ndl = max(dot(vNormal, normalize(lightPosition - vPositionW)), 0.0);
  frag_color = vec4(color * ndl, 1.0);
}
