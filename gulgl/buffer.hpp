#pragma once
#include <utility>

namespace gulgl {

  // TODO: Abstract GL_ARRAY_BUFFER

  enum class Draw_mode : GLenum {
    Points                   = GL_POINTS,
    Line_strip               = GL_LINE_STRIP,
    Line_loop                = GL_LINE_LOOP,
    Lines                    = GL_LINES,
    Line_strip_adjacency     = GL_LINE_STRIP_ADJACENCY,
    Line_adjacency           = GL_LINES_ADJACENCY,
    Triangle_strip           = GL_TRIANGLE_STRIP,
    Triangle_fan             = GL_TRIANGLE_FAN,
    Triangles                = GL_TRIANGLES,
    Triangle_strip_adjacency = GL_TRIANGLE_STRIP_ADJACENCY,
    Triangles_adjacency      = GL_TRIANGLES_ADJACENCY,
    Patches                  = GL_PATCHES };

  enum class Buffer_type : GLenum {
    Byte   = GL_BYTE,
    Ubyte  = GL_UNSIGNED_BYTE,
    Short  = GL_SHORT,
    Ushort = GL_UNSIGNED_SHORT,
    Int    = GL_INT,
    Uint   = GL_UNSIGNED_INT,
    Half   = GL_HALF_FLOAT,
    Float  = GL_FLOAT,
    Fixed  = GL_FIXED,
    Double = GL_DOUBLE };

  enum class Buffer_draw_kind : GLenum {
    Static_draw  = GL_STATIC_DRAW,
    Dynamic_draw = GL_DYNAMIC_DRAW,
    Stream_draw  = GL_STREAM_DRAW
  };

  template <typename Container>
  class Buffer {
    public:
      Buffer(Container data) {
        glGenBuffers(1, &id_);  // Generate 1 buffer
        bind();
        set(std::move(data));
      }

      void bind () const {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
      }

      void set(Container data) {
        data_ = std::move(data);
        reload_data();
      }

      [[nodiscard]] Container const & cdata () const { return data_; }
      [[nodiscard]] Container & data () { return data_; }

      void draw (GLuint attrib_array) const {
        glEnableVertexAttribArray(attrib_array);
        bind();
        glVertexAttribPointer(
           0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
           3,                  // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           nullptr             // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(attrib_array);
      }

    private:
      GLuint    id_{};
      Container data_;

      void reload_data () {
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(typename Container::value_type) * data_.size()),
            data_.data(),
            GL_STATIC_DRAW);
      }
  };

}
