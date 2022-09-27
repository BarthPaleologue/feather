#version 400

out vec4 frag_color;
in vec3 fColor;

varying vec3 vPosition;
varying vec3 vPositionW;

uniform vec3 lightPosition;

void main() {
  float ndl = max(dot(normalize(vPosition), normalize(lightPosition - vPositionW)), 0.0);
  frag_color = vec4(fColor * ndl, 1.0);
}
