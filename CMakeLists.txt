cmake_minimum_required(VERSION 3.10)
project("world-generator")

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible

file(MAKE_DIRECTORY ./build/)

IF (WIN32)
  # set stuff for windows
  file(MAKE_DIRECTORY "./build/windows/")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/windows/")
ELSE()
  file(MAKE_DIRECTORY "./build/linux/")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/linux/")
ENDIF()



include_directories("./libraries/glad/include/")

add_subdirectory("./libraries/glfw")



add_executable("world-generator" "./src/main.cpp" "./libraries/glad/src/glad.c")
target_link_libraries(world-generator glfw)
