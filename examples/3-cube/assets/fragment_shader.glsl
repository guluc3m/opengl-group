#version 330 core

in vec3 position;
out vec4 colour;

void main () {
  colour = vec4(0.5 * position + vec3(0.5, 0.5, 0.5), 1.0);
}
