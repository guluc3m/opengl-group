#version 330 core

uniform mat4 matrix;
layout(location = 0) in vec3 pos;
out vec3 my_pos;

void main(){
    my_pos = pos;
    gl_Position = matrix * vec4(pos, 1.0);
}

