add_executable(triangle triangle.cpp)
add_custom_target(move_assets_triangle ALL
   COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "${CMAKE_CURRENT_BINARY_DIR}/assets")
target_link_libraries(triangle gulgl)
add_dependencies(triangle move_assets_triangle)
