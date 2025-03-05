#pragma once

#include <utility>

namespace gulgl {

  class VAO {
    public:
      VAO () {
        glGenVertexArrays(1, &id_);
        bind();
      }
      void bind () const { glBindVertexArray(id_); }
      VAO (VAO && other) noexcept : id_{std::exchange(other.id_, 0)} { }
      VAO & operator= (VAO && other) noexcept {
        id_ = std::exchange(other.id_, 0);
        return *this;
      }
      ~VAO () noexcept = default;
      VAO (VAO const &) = delete;
      VAO & operator= (VAO const &) = delete;
    private:
      GLuint id_;
  };

}
