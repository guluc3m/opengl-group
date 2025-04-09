#pragma once
#include <type_traits>
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

  template <Buffer_type Type> struct Buffer_map;
  template <> struct Buffer_map<Buffer_type::Byte>   { using type = GLbyte;   };
  template <> struct Buffer_map<Buffer_type::Ubyte>  { using type = GLubyte;  };
  template <> struct Buffer_map<Buffer_type::Short>  { using type = GLshort;  };
  template <> struct Buffer_map<Buffer_type::Ushort> { using type = GLushort; };
  template <> struct Buffer_map<Buffer_type::Int>    { using type = GLint;    };
  template <> struct Buffer_map<Buffer_type::Uint>   { using type = GLuint;   };

  template <> struct Buffer_map<Buffer_type::Half>   { using type = GLhalf; };
  template <> struct Buffer_map<Buffer_type::Float>  { using type = GLfloat; };
  template <> struct Buffer_map<Buffer_type::Fixed>  { using type = GLfixed; };
  template <> struct Buffer_map<Buffer_type::Double> { using type = GLdouble; };

  enum class Buffer_draw_kind : GLenum {
    Static_draw  = GL_STATIC_DRAW,
    Dynamic_draw = GL_DYNAMIC_DRAW,
    Stream_draw  = GL_STREAM_DRAW
  };

  // template <Byffer_type type, std::size_t size, std::size_t stride = 0>
  // struct Buffer_definition {};

  namespace detail {
    template <auto N, typename T, auto Q>
    constexpr auto get_glm_vec_dimension (glm::vec<N, T, Q>) { return N; }

    template <auto N, typename T, auto Q>
    constexpr T get_glm_vec_type (glm::vec<N, T, Q>) { return std::declval<T>(); }

    template <typename T>
    constexpr std::size_t glm_vec_dimension =
      static_cast<std::size_t>(get_glm_vec_dimension(T{}));

    template <typename V>
    using glm_vec_type = decltype(get_glm_vec_type(std::declval<V>()));
  }

  template <typename Container>
  class SimpleBuffer {
      using vec_type = Container::value_type;
      using value_type = detail::glm_vec_type<vec_type>;
      static constexpr auto vec_size = detail::glm_vec_dimension<vec_type>;
    public:
      SimpleBuffer(Container data) {
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

      void bind (GLuint attrib_array) const {
        glEnableVertexAttribArray(attrib_array);
        bind();
        // static_assert(vec_size == 3);
        glVertexAttribPointer(
           attrib_array,       // attribute 0. No particular reason for 0, but must match the layout in the shader.
           vec_size,           // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           nullptr             // array buffer offset
        );
      }

      void draw () const {
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(data_.size()));
      }

      void unbind (GLuint attrib_array) const {
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
