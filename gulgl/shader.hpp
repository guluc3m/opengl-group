#pragma once

#include <gsl/gsl>
#include <sstream>
#include <string>
#include <concepts>
#include <istream>
#include <fstream>

namespace gulgl {

  namespace details {
    template <typename Getter>
      requires std::invocable<Getter, GLuint, GLenum, GLint*>
    void check_status (Getter get, GLuint item, GLuint field) {
      int log_length = 0;
      GLint result = GL_FALSE;
      get(item, field, &result);
      get(item, GL_INFO_LOG_LENGTH, &log_length);
      if (log_length > 0) {
        std::string message(static_cast<std::size_t>(log_length + 1), '\0');
        glGetShaderInfoLog(item, log_length, nullptr, message.data());
        throw GLError{message};
      }
    }
  }

  enum class Shader_kind : int {
    Vertex_shader   = GL_VERTEX_SHADER,
    Fragment_shader = GL_FRAGMENT_SHADER
  };

  class Program;

  template <Shader_kind shader_kind>
  class Shader {
    public:
      template <typename String_type>
      Shader (String_type data, bool is_file) {
        id_ = glCreateShader(static_cast<GLuint>(shader_kind));
        if (is_file) {
          std::ifstream file{data};
          if (not file) {
            std::string msg = "Couldn't open shader file: ";
            msg += std::string{data};
            throw GLError{msg};
          }
          std::string code(std::istreambuf_iterator<char>(file), {});
          gsl::czstring text{code.data()};
          glShaderSource(id_, 1, &text, nullptr);
        } else {
          gsl::czstring text{static_cast<const char*>(data)};
          glShaderSource(id_, 1, &text, nullptr);
        }
        glCompileShader(id_);
        details::check_status(glGetShaderiv, id_, GL_COMPILE_STATUS);
      }

      Shader (Shader const &) = delete;
      Shader & operator= (Shader const &) = delete;
      Shader (Shader &&) noexcept = default;
      Shader & operator= (Shader &&) noexcept = default;
      ~Shader () noexcept { glDeleteShader(id_); }
    private:
      GLuint id_;

      std::string readString (std::istream &input) {
        std::string code(std::istreambuf_iterator<char>(input), {});
        return code;
      }

      friend Program;
  };

}
