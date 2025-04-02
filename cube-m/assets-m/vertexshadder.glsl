#version 330 core
uniform mat4 matrix;
uniform mat4 MVP;
// Layout -> elegir canal por el que llega (es innecesario si location=0)
// VertexPos -> hacerlo en orden
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
// Se puede conseguir un objeto con las posiciones de vec3 para enviar a fragment shader. Se usa out para enviar
out vec3 my_pos;
out vec3 fragmentColor;
void main(){
    // xyz -> lugares del espacio
    // w -> 1 si es un punto, 0 si es una dirección
    my_pos = vertexPosition_modelspace;
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    fragmentColor = vertexColor;
}