// Include standard headers
#include <gulgl>
#include <vector>
#include <fstream>
#include <sstream>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(static_cast<size_t>(InfoLogLength+1));
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(static_cast<size_t>(InfoLogLength+1));
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(static_cast<size_t>(InfoLogLength+1));
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

int main () {
  gulgl::Window::hint(gulgl::Window_hint::Samples, 4);
  gulgl::Window::hint(gulgl::Window_hint::Context_version_major, 3);
  gulgl::Window::hint(gulgl::Window_hint::Context_version_minor, 3);
  gulgl::Window::hint(gulgl::Window_hint::OpenGL_forward_compat, GL_TRUE);
  gulgl::Window::hint(gulgl::Window_hint::OpenGL_profile, GLFW_OPENGL_CORE_PROFILE);

  gulgl::Window window{1024, 768, "Tutorial 01"};
  window.make_context_current();

	// Cargar los shaders (ruta absoluta)
  GLuint programID = LoadShaders("/home/mario/Projects/CLionProjects/gulgu/triangle/assets-m/vertexshadder.glsl",
  	"/home/mario/Projects/CLionProjects/gulgu/triangle/assets-m/fragmentshadder.glsl" );


  //VAO: abre un canal de comunicación entre cpu y gpu que nos permite pasar arrays
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  // An array of 3 vectors which represents 3 vertices
  static const GLfloat g_vertex_buffer_data[] = {
    0.8f, 0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
    -0.8f, 0.8f, 0.0f,
    -0.8f, 0.8f, 0.0f,
    -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
 };


  // Objeto para enviar los vertices hacia la tarjeta gráfica. Se debe especificar array porque podria ser un flujo de datos
  // This will identify our vertex buffer
  GLuint vertexbuffer;
  // Generar 1 buffer (pueden salir más)
  glGenBuffers(1, &vertexbuffer);
  // Guardar el vertexbuffer como array
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Enviar datos del array
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);



  // Ensure we can capture the escape key being pressed below
  window.set_input_mode(GLFW_STICKY_KEYS, true);
  do {
  		// Limpiar tanto el vértice como los shadders
  		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Estas lineas son para dibujar el triángulo
    // 1st attribute buffer : vertices
    // Index por el canal que se transmite
    glEnableVertexAttribArray(0);
    // Tipo del dato
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // número de elementos por cada array
       GL_FLOAT,           // tipo de los vertices
       GL_FALSE,           // es entre -1 y 1?
       0,                  // stride (ignorar por ahora, tiene algo que ver con el array)
       (void*)nullptr            // offset de cuando empieza cada elemento: solo util para stride
    );
    // Usando un shadder
  	glUseProgram(programID);
  	// Dibujar el triángulo
    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    window.swap_buffers();
    gulgl::Window::poll_events();
  } // Check if the ESC key was pressed or the window was closed
  while(window.get_key(GLFW_KEY_ESCAPE) != GLFW_PRESS and not window.should_close() );

}
