// https://learnopengl.com/Getting-started/Hello-Window

#include <iostream>
#include <array>
#include <vector>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::array<unsigned int, 2> windowDimensions;
std::array<unsigned int, 2> worldDimensions;

unsigned char RNGIncrementor = 11; // A nice prime number
unsigned int numOfRNGCalls = 0;

int getTimeMillisec()
{
    return 5;
    // TODO
}

unsigned int RNG(unsigned int high)
{
    auto seed = getTimeMillisec() + RNGIncrementor*numOfRNGCalls;
    return (seed*seed + seed) % high;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}



int main()
{
  std::cout << "Hello, World!" << std::endl;





  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);


  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }



  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



  glViewport(0, 0, 800, 600);



  while(!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }



  glfwTerminate();





  return 0;
}
