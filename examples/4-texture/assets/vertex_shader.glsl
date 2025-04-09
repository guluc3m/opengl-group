#version 330 core

layout (location = 0) in vec3 vertex_position;

out vec2 UV;
uniform mat4 MVP;

void main () {
  UV.x = ((vertex_position.x + 1) / 2) / 3 + 1.0/3;
  UV.y = ((vertex_position.y + 1) / 2) / 3 + 1.0/3;
  gl_Position = MVP * vec4(vertex_position, 1.0);
}
