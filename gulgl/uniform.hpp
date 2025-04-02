#pragma once

namespace gulgl {

  class Uniform {
    public:
      void set (glm::mat4 const& mat) {
        glUniformMatrix4fv(id_, 1, GL_FALSE, &mat[0][0]);
      }

      void set (glm::vec2 const& vec) {
        glUniform2fv(id_, 1, &vec[0]);
      }

    private:
      friend class Program;
      GLint id_;
      Uniform (GLint id) : id_{id} {}
  };

}
