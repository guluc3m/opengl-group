add_executable(cube cube.cpp)
add_custom_target(move_assets_cube ALL
   COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "${CMAKE_CURRENT_BINARY_DIR}/assets")
target_link_libraries(cube gulgl)
add_dependencies(cube move_assets_cube)
