#version 330 core

in vec3 my_pos;
out vec3 color;

void main(){
    color = (my_pos + vec3(0.5, 0.5, 0.5));
}