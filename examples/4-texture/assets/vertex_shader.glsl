#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 uv;

out vec2 UV;
uniform mat4 MVP;

void main () {
  UV = uv / 3 + vec2(1.0/3, 1.0/3);
  // UV = (vertex_position.xy + 1) / 2 / 3 + vec2(1.0/3, 1.0/3);
  // UV.y *= -1;
  gl_Position = MVP * vec4(vertex_position, 1.0);
}
