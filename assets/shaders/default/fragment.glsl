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
  vec3 color = texColor;
  frag_color = vec4(color, 1.0);
}
