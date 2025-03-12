#include <gulgl>
#include <vector>

int main (int, char * argv[]) {
  gulgl::Window::hint(gulgl::Window_hint::Samples, 4);
  gulgl::Window::hint(gulgl::Window_hint::Context_version_major, 3);
  gulgl::Window::hint(gulgl::Window_hint::Context_version_minor, 3);
  gulgl::Window::hint(gulgl::Window_hint::OpenGL_forward_compat, GL_TRUE);
  gulgl::Window::hint(gulgl::Window_hint::OpenGL_profile, GLFW_OPENGL_CORE_PROFILE);

  gulgl::Window window{1024, 768, "Example 3 -- Cube"};
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
  gulgl::Buffer vertices{points};

  // Ensure we can capture the escape key being pressed below
  window.set_input_mode(GLFW_STICKY_KEYS, true);
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
    vertices.draw(0);
    program.unbind();
    window.swap_buffers();
    window.poll_events();
  } while (window.get_key(GLFW_KEY_ESCAPE) != GLFW_PRESS and not window.should_close() );

  return 0;
}
