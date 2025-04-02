#version 330 core

layout (location = 0) in vec3 vertex_position;
out vec3 position;
uniform mat4 MVP;

void main () {
  position = vertex_position;
  gl_Position = MVP * vec4(vertex_position, 1.0);
}
