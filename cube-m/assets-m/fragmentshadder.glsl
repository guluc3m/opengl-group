#version 330 core
// pon vec4 (y un atributo más) para asignar transparencia
// recibimos el vec3 generado por vertex shader (debe tener el mismo nombre)
in vec3 my_pos;
// out para enviarlo al main de triangle-m
out vec3 color;
void main(){
  color = (my_pos + vec3(1,1,1))*0.5;
}