#include <gulgl>
#include <vector>

constexpr std::size_t width = 1024;
constexpr std::size_t height = 768;

int main (int, char * argv[]) {
  gulgl::Window::hint(gulgl::Window_hint::Samples, 4);
  gulgl::Window::hint(gulgl::Window_hint::Context_version_major, 3);
  gulgl::Window::hint(gulgl::Window_hint::Context_version_minor, 3);
  gulgl::Window::hint(gulgl::Window_hint::OpenGL_forward_compat, GL_TRUE);
  gulgl::Window::hint(gulgl::Window_hint::OpenGL_profile, GLFW_OPENGL_CORE_PROFILE);

  gulgl::Window window{width, height, "Example 4 -- Texture"};
  window.make_context_current();

  std::filesystem::path const binary_path{argv[0]};
  std::filesystem::path const assets{binary_path.parent_path()/"assets"};
  gulgl::Shader<gulgl::Shader_kind::Vertex_shader> vertex_shader{assets/"vertex_shader.glsl"};
  gulgl::Shader<gulgl::Shader_kind::Fragment_shader> fragment_shader{assets/"fragment_shader.glsl"};
  gulgl::Program program{vertex_shader, fragment_shader};

  gulgl::VAO vao{};
  std::vector<glm::vec3> points = {
    {-1.0f,-1.0f,-1.0f}, {-1.0f,-1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f},
    { 1.0f, 1.0f,-1.0f}, {-1.0f,-1.0f,-1.0f}, {-1.0f, 1.0f,-1.0f},

    { 1.0f,-1.0f, 1.0f}, {-1.0f,-1.0f,-1.0f}, { 1.0f,-1.0f,-1.0f},
    { 1.0f, 1.0f,-1.0f}, { 1.0f,-1.0f,-1.0f}, {-1.0f,-1.0f,-1.0f},

    {-1.0f,-1.0f,-1.0f}, {-1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f,-1.0f},
    { 1.0f,-1.0f, 1.0f}, {-1.0f,-1.0f, 1.0f}, {-1.0f,-1.0f,-1.0f},

    {-1.0f, 1.0f, 1.0f}, {-1.0f,-1.0f, 1.0f}, { 1.0f,-1.0f, 1.0f},
    { 1.0f, 1.0f, 1.0f}, { 1.0f,-1.0f,-1.0f}, { 1.0f, 1.0f,-1.0f},

    { 1.0f,-1.0f,-1.0f}, { 1.0f, 1.0f, 1.0f}, { 1.0f,-1.0f, 1.0f},
    { 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f,-1.0f}, {-1.0f, 1.0f,-1.0f},

    { 1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f,-1.0f}, {-1.0f, 1.0f, 1.0f},
    { 1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}, { 1.0f,-1.0f, 1.0f}};

  std::vector<glm::vec2> texture_coords {
    {0.00f, 1.00f}, {0.00f, 0.66f}, {0.34f, 0.66f},
    {1.00f, 1.00f}, {0.67f, 0.66f}, {1.00f, 0.66f},

    {0.67f, 0.66f}, {0.34f, 0.33f}, {0.67f, 0.33f},
    {1.00f, 1.00f}, {0.67f, 1.00f}, {0.67f, 0.66f},

    {0.00f, 1.00f}, {0.34f, 0.66f}, {0.34f, 1.00f},
    {0.67f, 0.66f}, {0.34f, 0.66f}, {0.34f, 0.33f},

    {1.00f, 0.33f}, {1.00f, 0.66f}, {0.67f, 0.66f},
    {0.67f, 1.00f}, {0.34f, 0.66f}, {0.67f, 0.66f},

    {0.34f, 0.66f}, {0.67f, 1.00f}, {0.34f, 1.00f},
    {0.00f, 0.66f}, {0.00f, 0.33f}, {0.34f, 0.33f},

    {0.00f, 0.66f}, {0.34f, 0.33f}, {0.34f, 0.66f},
    {0.67f, 0.33f}, {1.00f, 0.33f}, {0.67f, 0.66f}};

  gulgl::SimpleBuffer vertices{points};
  gulgl::SimpleBuffer uv{texture_coords};

  gulgl::Uniform mvp_uniform = program.get_uniform("MVP");
  glm::mat4 mvp(1.0f);
  gulgl::Texture texture{assets/"cajoy.bmp"};
  gulgl::Uniform texture_uniform = program.get_uniform("texture_sampler");

  // Ensure we can capture the escape key being pressed below
  window.set_input_mode(GLFW_STICKY_KEYS, true);
  float rot = 0.0f;
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    program.bind();
    rot += 0.005f;

    // Set the matrix
    glm::mat4 proj =
      glm::perspective(glm::radians(45.0f),
      static_cast<float>(width) / height,
      0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
      glm::vec3(4,3,3),
      glm::vec3(0,0,0),
      glm::vec3(0,1,0)
    );
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(1.0));
    mvp = proj * view * model;
    mvp_uniform.set(mvp);

    // Set the texture
    texture.bind();
    texture_uniform.set(GLint{0});

    vertices.bind(0);
    uv.bind(1);
    
    vertices.draw();

    vertices.unbind(0);
    uv.unbind(1);

    // Unbind and deattivate things
    texture.unbind();
    program.unbind();

    // Swap buffers
    window.swap_buffers();
    window.poll_events();
  } while (window.get_key(GLFW_KEY_ESCAPE) != GLFW_PRESS and not window.should_close() );

  return 0;
}
