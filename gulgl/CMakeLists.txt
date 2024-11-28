add_library(gulgl INTERFACE)
target_include_directories(gulgl INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_sources(gulgl
   PUBLIC
      gulgl
   PRIVATE
      window.hpp)

target_link_libraries(gulgl PUBLIC
      glfw
      GLEW::GLEW
      Microsoft.GSL::GSL
      ${OPENGL_LIBRARIES})
