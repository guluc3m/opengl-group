#pragma once

#include <iostream>
#include "shader.hpp"
#include "uniform.hpp"
#include "errors.hpp"

namespace gulgl {

  class Program {
    public:
      Program (Shader<Shader_kind::Vertex_shader> & vertex_shader,
               Shader<Shader_kind::Fragment_shader> & fragment_shader) {
        id_ = glCreateProgram();
        glAttachShader(id_, vertex_shader.id_);
        glAttachShader(id_, fragment_shader.id_);
        glLinkProgram(id_);
        details::check_status(glGetProgramiv, id_, GL_LINK_STATUS);
        glDetachShader(id_, vertex_shader.id_);
        glDetachShader(id_, fragment_shader.id_);
      }

      Program (Program const &) = delete;
      Program & operator= (Program const &) = delete;
      Program (Program &&) noexcept = default;
      Program & operator= (Program &&) noexcept = default;
      ~Program () noexcept { glDeleteProgram(id_); }

      void bind () const { glUseProgram(id_); }
      void unbind () const { glUseProgram(0); }

      template <typename T>
      Uniform get_uniform (T name) {
        GLint const id = glGetUniformLocation(id_, name);
        if (id < 0) { throw GLError{"Uniform doesn't exist!"}; }
        return Uniform{id};
      }

    private:
      GLuint id_;
  };

}
