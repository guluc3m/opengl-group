cmake_minimum_required(VERSION 3.22)
project(opengl-group LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors
   -Wconversion -Wsign-conversion)

include(FetchContent)
FetchContent_Declare(GSL
   GIT_REPOSITORY "https://github.com/microsoft/GSL"
   GIT_TAG v4.0.0
   GIT_SHALLOW ON)
FetchContent_MakeAvailable(GSL)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(gulgl)
add_subdirectory(triangle)
add_subdirectory(cube)
add_subdirectory(window)
