#pragma once

#include "errors.hpp"

#include <iostream>
#include <cstdint>
#include <vector>

namespace gulgl {

  namespace detail {

    // Read: Base case

    template <typename Input_stream, typename Arg>
    bool read_one (Input_stream & is, std::ptrdiff_t & offset, Arg & arg) {
      offset += static_cast<std::ptrdiff_t>(sizeof(Arg));
      return static_cast<bool>(is.read(reinterpret_cast<char*>(&arg), sizeof(Arg)));
    }

    template <typename Input_stream>
    bool read_one (Input_stream & is, std::ptrdiff_t & offset, std::string & arg) {
      offset += std::ssize(arg);
      return static_cast<bool>(is.read(arg.data(), std::ssize(arg)));
    }

    template <typename Input_stream, typename T>
    bool read_one (Input_stream & is, std::ptrdiff_t & offset, std::vector<T> & arg) {
      auto size = std::ssize(arg) * static_cast<std::ptrdiff_t>(sizeof(T));
      offset += size;
      return static_cast<bool>(is.read(reinterpret_cast<char*>(arg.data()), size));
    }

    // Read: variadic

    template <typename Input_stream, typename ... Args>
    bool read (Input_stream & is, std::ptrdiff_t & offset, Args & ... args) {
      return (read_one(is, offset, args) and ...);
    }

    template <typename Input_stream, typename ... Args>
    void do_read (Input_stream & is, std::ptrdiff_t & offset, Args & ... args) {
      if (not read(is, offset, args ...)) {
        throw GLError{"Error while reading from file!"};
      }
    }

    template <typename Input_stream>
    void do_skip (Input_stream & is, std::ptrdiff_t & offset, std::ptrdiff_t to) {
      char dummy = 0;
      while (offset < to) { do_read(is, offset, dummy); }
    }

    template <typename String_type>
    GLuint texture_load_BMP (String_type const * path) {
      constexpr std::ptrdiff_t magic_size        = 2;
      constexpr std::ptrdiff_t header_size       = 54;
      constexpr std::ptrdiff_t data_pos_offset   = 0x0A;
      constexpr std::ptrdiff_t img_width_offset  = 0x12;
      constexpr std::ptrdiff_t img_size_offset   = 0x22;

      std::ifstream file{path, std::ios::binary};
      if (not file) { throw GLError{"Cannot open the file"}; }

      // Read the header
      std::ptrdiff_t offset = 0x00;
      std::string header(magic_size, '\0');
      do_read(file, offset, header);
      if (header != "BM") { throw GLError{"Invalid BMP header"}; }

      // Read the sizes
      int32_t data_pos = 0, image_size = 0, width = 0, height = 0;
      do_skip(file, offset, data_pos_offset);
      do_read(file, offset, data_pos);
      do_skip(file, offset, img_width_offset);
      do_read(file, offset, width, height);
      do_skip(file, offset, img_size_offset);
      do_read(file, offset, image_size);

      if (image_size == 0) { image_size = width * height * 3; }
      if (data_pos == 0)   { data_pos = header_size; }

      std::vector<unsigned char> data(static_cast<std::size_t>(image_size), 0);
      do_skip(file, offset, data_pos);
      do_read(file, offset, data);

      // Load it to OpenGL
      GLuint id = 0;
      glGenTextures(1, &id);
      glBindTexture(GL_TEXTURE_2D, id);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                   GL_BGR, GL_UNSIGNED_BYTE, data.data());

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap(GL_TEXTURE_2D);

      return id;
    }

  }

  class Texture {
    public:
      template <typename String_type>
      Texture (String_type path) {
        if constexpr (std::same_as<const char*, String_type>) {
          gsl::czstring text{path};
          id_ = detail::texture_load_BMP(text);
        } else {
          gsl::czstring text{path.c_str()};
          id_ = detail::texture_load_BMP(text);
        }
      }

      void bind() {
        // TODO: Change to other values
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id_);
      }

      void unbind () {
        glBindTexture(GL_TEXTURE_2D, 0);
      }

      ~Texture() { if (id_) { glDeleteTextures(1, &id_); } }
      Texture(Texture const& other) = delete;
      Texture & operator= (Texture const& other) = delete;
      Texture(Texture && other) : id_{std::exchange(other.id_, 0)} { }
      Texture & operator= (Texture && other) {
        id_ = std::exchange(other.id_, 0);
        return *this;
      }

    private:
      GLuint id_;
  };

}
