#version 400

out vec4 frag_color;
in vec3 fColor;

void main() {
  frag_color = vec4(fColor, 1.0);
}
