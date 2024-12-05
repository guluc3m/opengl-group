#version 330 core
// Layout -> elegir canal por el que llega (es innecesario si location=0)
// VertexPos -> hacerlo en orden
layout(location = 0) in vec3 vertexPosition_modelspace;
// Se puede conseguir un objeto con las posiciones de vec3 para enviar a fragment shader. Se usa out para enviar
out vec3 my_pos;
void main(){
    // xyz -> lugares del espacio
    // w -> 1 si es un punto, 0 si es una dirección
    my_pos = vertexPosition_modelspace;
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
}