#include <exception>
#include <memory>
#include <iostream>

namespace gulgl {

  enum class Window_hint : int {
    Focused = GLFW_FOCUSED,
    Iconified = GLFW_ICONIFIED,
    Resizable = GLFW_RESIZABLE,
    Visible = GLFW_VISIBLE,
    Decorated = GLFW_DECORATED,
    Auto_iconify = GLFW_AUTO_ICONIFY,
    Floating = GLFW_FLOATING,
    Maximized = GLFW_MAXIMIZED,
    Center_cursor = GLFW_CENTER_CURSOR,
    Transparent_framebuffer = GLFW_TRANSPARENT_FRAMEBUFFER,
    Hovered = GLFW_HOVERED,
    Focus_on_show = GLFW_FOCUS_ON_SHOW,
    Red_bits = GLFW_RED_BITS,
    Green_bits = GLFW_GREEN_BITS,
    Blue_bits = GLFW_BLUE_BITS,
    Alpha_bits = GLFW_ALPHA_BITS,
    Depth_bits = GLFW_DEPTH_BITS,
    Stencil_bits = GLFW_STENCIL_BITS,
    Accum_red_bits = GLFW_ACCUM_RED_BITS,
    Accum_green_bits = GLFW_ACCUM_GREEN_BITS,
    Accum_blue_bits = GLFW_ACCUM_BLUE_BITS,
    Accum_alpha_bits = GLFW_ACCUM_ALPHA_BITS,
    Aux_buffers = GLFW_AUX_BUFFERS,
    Stereo = GLFW_STEREO,
    Samples = GLFW_SAMPLES,
    Srgb_capable = GLFW_SRGB_CAPABLE,
    Refresh_rate = GLFW_REFRESH_RATE,
    Doublebuffer = GLFW_DOUBLEBUFFER,
    Client_api = GLFW_CLIENT_API,
    Context_version_major = GLFW_CONTEXT_VERSION_MAJOR,
    Context_version_minor = GLFW_CONTEXT_VERSION_MINOR,
    Context_revision = GLFW_CONTEXT_REVISION,
    Context_robustness = GLFW_CONTEXT_ROBUSTNESS,
    OpenGL_forward_compat = GLFW_OPENGL_FORWARD_COMPAT,
    OpenGL_debug_context = GLFW_OPENGL_DEBUG_CONTEXT,
    OpenGL_profile = GLFW_OPENGL_PROFILE,
    Context_release_behavior = GLFW_CONTEXT_RELEASE_BEHAVIOR,
    Context_no_error = GLFW_CONTEXT_NO_ERROR,
    Context_creation_api = GLFW_CONTEXT_CREATION_API,
    Scale_to_monitor = GLFW_SCALE_TO_MONITOR,
    Cocoa_retina_framebuffer = GLFW_COCOA_RETINA_FRAMEBUFFER,
    Cocoa_frame_name = GLFW_COCOA_FRAME_NAME,
    Cocoa_graphics_switching = GLFW_COCOA_GRAPHICS_SWITCHING,
    X11_class_name = GLFW_X11_CLASS_NAME,
    X11_instance_name = GLFW_X11_INSTANCE_NAME
  };

  class Window {
    public:
      // Window
      Window(int width, int height, std::convertible_to<const char*> auto const title) {
        init_glfw();
        window_.reset(glfwCreateWindow(width, height, title, nullptr, nullptr));
        make_context_current();
        if (not window_) {
          std::cerr << "Failed to open GLFW window\n";
          std::terminate();
        }
        init_glew();
      }

      void make_context_current () { glfwMakeContextCurrent(window_.get()); }
      void set_input_mode(int name, bool mode) {
        glfwSetInputMode(window_.get(), name, mode); }
      void swap_buffers() { glfwSwapBuffers(window_.get()); }
      bool should_close() { return 0 != glfwWindowShouldClose(window_.get()); }
      auto get_key (int key) { return glfwGetKey(window_.get(), key); }
  
      // Static functions
      static void hint (Window_hint hint, int value) {
        glfwWindowHint(static_cast<int>(hint), value);
      }
      static void hint (Window_hint hint, std::convertible_to<const char*> auto const & name) {
        glfwWindowHintString(static_cast<int>(hint), name);
      }
      static void poll_events() { glfwPollEvents(); }
    private:
      struct Window_deleter {
        void operator () (GLFWwindow * w) const { glfwDestroyWindow(w); }
      };

      static void init_glfw () {
        glewExperimental = true;
        if (not glfwInit()) {
          std::cerr << "Failed to initialise GLFW\n";
          std::terminate();
        }
      }

      static void init_glew () {
        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
          std::cerr << "Failed to initialize GLEW\n";
          std::terminate();
        }
      }

      std::unique_ptr<GLFWwindow, Window_deleter> window_{};
  };
}
