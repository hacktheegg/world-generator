cmake_minimum_required(VERSION 3.10)
project("world-generator")

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible

file(MAKE_DIRECTORY ./build/)

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")

IF (WIN32)
  # set stuff for windows
  file(MAKE_DIRECTORY "./build/windows/")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/windows/")
ELSE()
  # set stuff for linux
  file(MAKE_DIRECTORY "./build/linux/")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/linux/")
ENDIF()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include_directories("${CMAKE_BINARY_DIR}/libraries/glad/include/")

add_subdirectory("${CMAKE_BINARY_DIR}/libraries/glfw/")

configure_file("${CMAKE_BINARY_DIR}/src/shaders/shader.vert" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
configure_file("${CMAKE_BINARY_DIR}/src/shaders/shader.frag" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


add_executable("world-generator" "${CMAKE_BINARY_DIR}/src/main.cpp" "${CMAKE_BINARY_DIR}/libraries/glad/src/glad.c")
target_link_libraries(world-generator glfw)
