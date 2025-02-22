// https://learnopengl.com/Getting-started/Hello-Window

#include <cmath>
#include <iostream>
#include <array>
#include <vector>
#include <chrono>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using ByteMatrix = std::vector<std::vector<unsigned char>>;

const float pi = 3.14159265;

std::array<unsigned int, 2> windowDimensions;
std::array<unsigned int, 2> worldDimensions;

unsigned char RNGIncrementor = 11; // A nice prime number
unsigned int numOfRNGCalls = 0;

uint64_t getTimeMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

unsigned int RNG(unsigned int high)
{
    unsigned int seed = getTimeMillisec() * numOfRNGCalls;
    numOfRNGCalls += 1;
    for (int i = 0; i < 3; i++)
    {
      if (!(seed % 2))
      {
        seed /= 2;
      }
      else
      {
        seed *= 3;
        seed += 1;
      }
    }
    return (seed % high);
}

ByteMatrix generateNoise(unsigned int width, unsigned int height, float density /*0 -> 1; allblack->all white*/, unsigned char smoothness = 0)
{
  ByteMatrix noiseMap = {};

  // initialise noiseMap a matrix of 0's
  std::vector<unsigned char> tempVec = {};
  for (unsigned int i = 0; i < height; i++)
  {
    for (unsigned int ii = 0; ii < width; ii++)
    {
      tempVec.push_back(0);
    }
    noiseMap.push_back(tempVec);
    tempVec = {};
  }
  
  // place bright spots on noiseMap
  for (unsigned long i = 0; i < density*width*height; i++)
  {
    noiseMap[RNG(height)][RNG(width)] = 0xFF;
  }
  
  return noiseMap;
}

void printMatrixInTerminal(ByteMatrix matrix)
{
  for(auto& line : matrix)
  {
    for (auto num : line)
    {
      if (num == 0)
      {
        std::cout << "  ";
      }
      else
      {
        std::cout << "# ";
      }
    }
    std::cout << std::endl;
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}




int main()
{
  std::cout << "Hello, World!" << std::endl;

  printMatrixInTerminal(generateNoise(183,81,0.01));




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



  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };



  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(
      (sin(glfwGetTime()+(3.14f/3*3))+1.0f)/2.0f, // red
      (sin(glfwGetTime()+(3.14f/3*2))+1.0f)/2.0f, // green
      (sin(glfwGetTime()+(3.14f/3*1))+1.0f)/2.0f, // blue
      1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // std::cout << "sin: " << (sin(glfwGetTime())+1.0f)/2.0f << std::endl;

    glfwSwapBuffers(window);
    glfwPollEvents();    
  }



  glfwTerminate();





  return 0;
}
