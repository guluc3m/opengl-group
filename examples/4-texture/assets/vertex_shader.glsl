#version 330 core

layout (location = 0) in vec3 vertex_position;

out vec2 UV;
uniform mat4 MVP;

void main () {
  UV = vertex_position.xy;
  gl_Position = MVP * vec4(vertex_position, 1.0);
}
