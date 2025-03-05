#pragma once

#include <gsl/gsl>

namespace gulgl {

  class Base_error : public std::exception {
    public:
      Base_error (std::string & msg) : msg_{std::move(msg)} {}

      Base_error (gsl::czstring msg) : msg_{msg} {}
      // NOLINTNEXTLINE (cpp-coreguidelines-pro-type-reinterpret-cast)
      Base_error (const unsigned char * msg) : msg_{reinterpret_cast<gsl::czstring>(msg)} {}

      [[nodiscard]] gsl::czstring what () const noexcept override { return msg_.data(); }
    private:
      std::string msg_;
  };

  class GLFWError : public Base_error { using Base_error::Base_error; };
  class GLEWError : public Base_error { using Base_error::Base_error; };
  class GLError   : public Base_error { using Base_error::Base_error; };
}
