#version 330 core

layout(location = 0) in vec3 pos;
out vec3 my_pos;

void main(){
    my_pos = pos;
    //if (my_pos.z == 0.0) { my_pos.z = 1.0; }
    gl_Position = vec4(pos, 1.0);
}

