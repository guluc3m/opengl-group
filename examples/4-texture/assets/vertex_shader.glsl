#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 uv;

out vec2 UV;
uniform mat4 MVP;

void main () {
  UV = uv;
  gl_Position = MVP * vec4(vertex_position, 1.0);
}
