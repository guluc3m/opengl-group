//
// Created by golden on 11/28/24.
//

#define GLM_ENABLE_EXPERIMENTAL
// Include standard headers
#include <gulgl>
#include <vector>
#include <fstream>
#include <sstream>
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
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer	 , nullptr);
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

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "/home/golden/GUL/openGL/opengl-group/assets/vertex_shader.glsl", "/home/golden/GUL/openGL/opengl-group/assets/fragment_shader.glsl");
	glUseProgram(programID);
    // VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Matrices de transformación
	GLint matrix_id = glGetUniformLocation(programID, "matrix");
	glm::mat4 model = glm::mat4(1.0f);
	// glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);
	// glm::mat4 translation = glm::mat4(1.0f);

    // Ensure we can capture the escape key being pressed below
    window.set_input_mode(GLFW_STICKY_KEYS, true);
	float angle = 0.0; // Angulo para rotacion
    do {

    	// Enable depth test
    	glEnable(GL_DEPTH_TEST);
    	// Accept fragment if it closer to the camera than the former one
    	glDepthFunc(GL_LESS);

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// Use our shader
    	glUseProgram(programID);
		// Matrices usadas
    	rotation = glm::rotate(angle += 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
    	model = rotation;

    	// Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
    	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 1024 / (float)768, 0.1f, 100.0f);

    	// Or, for an ortho camera:
    	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    	// Camera matrix
    	glm::mat4 View = glm::lookAt(
			glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);

    	// Our ModelViewProjection: multiplication of our 3 matrices
    	glm::mat4 mvp = Projection * View * model; // Remember, matrix multiplication is the other way around

    	glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
           0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
           3,                  // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           nullptr            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
        glDisableVertexAttribArray(0);

        window.swap_buffers();
        gulgl::Window::poll_events();
    } // Check if the ESC key was pressed or the window was closed
    while(window.get_key(GLFW_KEY_ESCAPE) != GLFW_PRESS and not window.should_close() );

}