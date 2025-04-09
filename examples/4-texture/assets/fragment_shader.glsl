#version 330 core

in vec2 UV;
out vec4 colour;
uniform sampler2D texture_sampler;

void main () {
  colour = vec4(texture(texture_sampler, UV).rgb, 0);
}
