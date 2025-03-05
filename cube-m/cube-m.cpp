// Include standard headers
#include <gulgl>
#include <vector>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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

void computeMatricesFromInputs() {
	// position
	glm::vec3 position = glm::vec3( 0, 0, 5 );
	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.005f;
	// Get mouse position
	int xpos, ypos;
	glfwGetMousePos(&xpos, &ypos);
	// Reset mouse position for next frame
	glfwSetMousePos(1024/2, 768/2);

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
  GLuint programID = LoadShaders("/home/daniel/opengl-group/cube-m/assets-m/vertexshadder.glsl",
  	"/home/daniel/opengl-group/cube-m/assets-m/fragmentshadder.glsl" );


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


  // Objeto para englGetUniformLocaviar los vertices hacia la tarjeta gráfica. Se debe especificar array porque podria ser un flujo de datos
  // This will identify our vertex buffer
  GLuint vertexbuffer;
  // Generar 1 buffer (pueden salir más)
  glGenBuffers(1, &vertexbuffer);
  // Guardar el vertexbuffer como array
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Enviar datos del array
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLint matrix_id = glGetUniformLocation(programID, "matrix");
	//glm::mat4 matrix = glm::mat4(1.0f);
	//glm::mat4 traslation = glm::mat4(1.0f);
	//glm::mat4 scale = glm::mat4(1.0f);
	//glm::mat4 rotation = glm::mat4(1.0f);

  // Ensure we can capture the escape key being pressed below
  window.set_input_mode(GLFW_STICKY_KEYS, true);
	//float angle = 0.0;
  do {
  		// Limpiar tanto el vértice como los shadders
  		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  	glUseProgram(programID);

  	// Compute the MVP matrix from keyboard and mouse input
  		computeMatricesFromInputs();
  		glm::mat4 ProjectionMatrix = getProjectionMatrix();
  		glm::mat4 ViewMatrix = getViewMatrix();
  		glm::mat4 ModelMatrix = glm::mat4(1.0);
  		glm::mat4 matrix = ProjectionMatrix * ViewMatrix * ModelMatrix;


  		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &matrix[0][0]);

		// Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 1024/ (float)768, 0.1f, 100.0f);

		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);

		// Model matrix: an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);
		// Our ModelViewProjection: multiplication of our 3 matrices
		glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
		
		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLint MatrixID = glGetUniformLocation(programID, "MVP");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		//computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = Projection;
		glm::mat4 ViewMatrix = View;
		glm::mat4 ModelMatrix = Model;
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glm::vec3 position = glm::vec3( 0, 0, 5 );
		// horizontal angle : toward -Z
		float horizontalAngle = 3.14f;
		// vertical angle : 0, look at the horizon
		float verticalAngle = 0.0f;
		// Initial Field of View
		float initialFoV = 45.0f;

		float speed = 3.0f; // 3 units / second
		float mouseSpeed = 0.005f;
		
		// Get mouse position
		//int xpos, ypos;
		//glfwGetC(&xpos, &ypos);


			

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
  	// Dibujar el triángulo
    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

		window.swap_buffers();
		gulgl::Window::poll_events();
  } // Check if the ESC key was pressed or the window was closed
  while(window.get_key(GLFW_KEY_ESCAPE) != GLFW_PRESS and not window.should_close() );

}
