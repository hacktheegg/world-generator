// https://learnopengl.com/Getting-started/Hello-Window

#include <cmath>
#include <iostream>
#include <array>
#include <vector>
#include <chrono>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.hpp"
#include "currentDir.hpp"

using ByteMatrix = std::vector<std::vector<unsigned char>>;

const float pi = 3.14159265;

std::array<unsigned int, 2> windowDimensions;
std::array<unsigned int, 2> worldDimensions;

unsigned int numOfRNGCalls = 0;

uint64_t getTimeMillisec() 
{
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

ByteMatrix generateNoise(unsigned int width, unsigned int height, float density /*0 -> 1; allblack->all white*/, unsigned char smoothness = 3)
{
  ByteMatrix noiseMap(height, std::vector<unsigned char>(width, 0)); // initialise noiseMap a matrix of 0's
  
  // place bright spots on noiseMap
  for (unsigned long i = 0; i < density*width*height; i++)
  {
    noiseMap[RNG(height)][RNG(width)] = 0xFF;
  }
  
  // Apply blur
  float brightnessWeight = 0.0f; // How much pixels that make the blur brighter are favoured // TODO
  unsigned short localBrightnessSum;

  while (smoothness > 1)
  {
    // Blur horizontally //
    for (unsigned int y = 0; y < height; y++)
    {
      for (unsigned int x = 0; x < std::ceil(width/smoothness); x++)
      {
        for (unsigned int i = 0; i < smoothness; i++)
        {
          if (x*smoothness+i > width) 
          {
            localBrightnessSum += localBrightnessSum / i; // To prevent pixels being too dim after the average is set
            break; // Slightly hacky but works theoretically (i havent tested it)
          }
          localBrightnessSum += noiseMap[y][x*smoothness+i];
        }
        for (unsigned int i = 0; i < smoothness; i++)
        {
          noiseMap[y][x*smoothness+i] = localBrightnessSum / smoothness; 
        }
        localBrightnessSum = 0;
      }
    }
    
    // Blur vertically //
    for (unsigned int x = 0; x < width; x++)
    {
      for (unsigned int y = 0; y < std::ceil(height/smoothness); y++)
      {
        for (unsigned int i = 0; i < smoothness; i++)
        {
          if (y*smoothness+i > height) 
          {
            localBrightnessSum += localBrightnessSum / i; // 
            break; // 
          }
          localBrightnessSum += noiseMap[y*smoothness+i][x];
        }
        for (unsigned int i = 0; i < smoothness; i++)
        {
          noiseMap[y*smoothness+i][x] = localBrightnessSum / smoothness; 
        }
        localBrightnessSum = 0;
      }
    }
    
    smoothness -= 1;
  }
  // END of blur code //
  
  return noiseMap;
}

void printMatrixInTerminal(ByteMatrix matrix)
{
  for(auto& line : matrix)
  {
    for (auto num : line)
    {
      std::cout << (int)num+1 << ' ';
    }
    std::cout << std::endl;
  }
}




// Shape Drawing Functions //
void drawSquare(std::array<unsigned int, 2> coord1, std::array<unsigned int, 2> coord2, std::array<unsigned char, 3> RGB)
{
  // TODO
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}



const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";




int main()
{
  std::cout << "Hello, World!" << std::endl;

  //printMatrixInTerminal(generateNoise(83,181,0.01, 5));



  // initialise GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  // initialise GLFW window
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // initialise GLAD function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  //// SHADERS ////
  Shader ourShader("./shader.vert", "./shader.frag");

  // vertices and vertice attributes
  float vertices[] = {
    -0.75f,  0.75f, 0.0f,  0.00f,  0.75f, 0.0f,  0.75f,  0.75f, 0.0f,
    -0.75f,  0.00f, 0.0f,  0.00f,  0.00f, 0.0f,  0.75f,  0.00f, 0.0f,
    -0.75f, -0.75f, 0.0f,  0.00f, -0.75f, 0.0f,  0.75f, -0.75f, 0.0f
  };
  unsigned int indices[] = {
    0, 1, 3,
    1, 3, 4,
    4, 5, 7,
    5, 7, 8
  };  
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  //glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



  glViewport(0, 0, 800, 600);


  std::cout << filepath::exePath() << std::endl;


  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(
      (sin(glfwGetTime()+(3.14f/3*3))+1.0f)/2.0f, // red
      (sin(glfwGetTime()+(3.14f/3*2))+1.0f)/2.0f, // green
      (sin(glfwGetTime()+(3.14f/3*1))+1.0f)/2.0f, // blue
      1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // std::cout << "sin: " << (sin(glfwGetTime())+1.0f)/2.0f << std::endl;
  


    // draw our first triangle
    ourShader.use();
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0); // no need to unbind it every time



    glfwSwapBuffers(window);
    glfwPollEvents();    
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);


  glfwTerminate();
  return 0;
}
