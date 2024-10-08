add_library(gulgl
      dummy.cpp)
target_link_libraries(gulgl PUBLIC
      glfw
      GLEW::GLEW
      Microsoft.GSL::GSL
      ${OPENGL_LIBRARIES})
