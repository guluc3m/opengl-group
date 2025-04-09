add_executable(texture texture.cpp)
add_custom_target(move_assets_texture ALL
   COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "${CMAKE_CURRENT_BINARY_DIR}/assets")
target_link_libraries(texture gulgl)
add_dependencies(texture move_assets_texture)
