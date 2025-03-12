#include <gulgl>

using namespace gulgl;

int main () {
  Window::hint(Window_hint::Samples, 4);
  Window::hint(Window_hint::Context_version_major, 3);
  Window::hint(Window_hint::Context_version_minor, 3);
  Window::hint(Window_hint::OpenGL_forward_compat, GL_TRUE);
  Window::hint(Window_hint::OpenGL_profile, GLFW_OPENGL_CORE_PROFILE);

  Window window{1024, 768, "Example 1 -- Window"};
  window.make_context_current();

  // Ensure we can capture the escape key being pressed below
  window.set_input_mode(GLFW_STICKY_KEYS, true);
  do {
    glClear(GL_COLOR_BUFFER_BIT);

    window.swap_buffers();
    window.poll_events();
  } // Check if the ESC key was pressed or the window was closed
  while(window.get_key(GLFW_KEY_ESCAPE) != GLFW_PRESS
        and not window.should_close() );

}
