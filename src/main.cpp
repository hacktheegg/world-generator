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
//#include "currentDir.hpp"

using ByteMatrix = std::vector<std::vector<unsigned char>>;
using RGBColour = std::array<unsigned char, 3>;

const float pi = 3.14159265;
const float e = 2.718281828;
const float pie = pi + e; // I *promise* to use this!

std::array<unsigned int, 2> windowDimensions;
std::array<unsigned int, 2> worldDimensions;

std::array<long, 2> cameraPosition = {0,0}; // where the visible portion of the world is centred // TODO: implement
unsigned int viewSize = 12; // The radius of the visible portion of the world (it's a rectangle...)

unsigned int numOfRNGCalls = 0;

uint64_t getTimeMicrosec() 
{
  using namespace std::chrono;
  return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
}


unsigned int RNG(unsigned int high)
{
    unsigned int seed = getTimeMicrosec() * numOfRNGCalls;
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
            break; // Slightly hacky but works 
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
      if (num < 9) std::cout << (int)num+1 << "   ";
      else if (num < 99) std::cout << (int)num+1 << "  ";
      else std::cout << (int)num+1 << ' ';
    }
    std::cout << '\n';
  }
}

void printCameraViewInTerminal(const ByteMatrix matrix)
{
  for (unsigned int i = 0; i < viewSize*2; i++)
  {
    long absPosY = cameraPosition[1] - viewSize + i;
    
    if (absPosY < 0 || absPosY > matrix.size()) // Draw the entire row blank if its outside the bounds
    {
      for (unsigned int n = 0; n < viewSize*2; n++) {std::cout << "0   ";}
      std::cout << '\n';
      continue;
    } 

    for (unsigned int j = 0; j < viewSize*2; j++)
    {
      long absPosX = cameraPosition[0] - viewSize + j;

      if (absPosX < 0 || absPosX > matrix[0].size()) {std::cout << "0   "; continue;}
      std::cout << (int)matrix[absPosY][absPosX] + 1;
      if (matrix[absPosY][absPosX] < 9) std::cout << "   ";
      else if (matrix[absPosY][absPosX] < 99) std::cout << "  ";
      else std::cout << ' ';
    }
    std::cout << '\n';
  }
}

// Biome Classification stuff
RGBColour colourPixelByBiome(unsigned char pixelBrightness) // TODO: Improve func name
{
  if (pixelBrightness < 0x10) return {0, 80, 100}; // Water
  if (pixelBrightness < 0x20) return {100, 20, 0}; // Beach
  return {0, 120, 200}; // Land
}


// Shape Drawing Functions //
void drawSquare(std::array<unsigned int, 2> coord1, std::array<unsigned int, 2> coord2, RGBColour RGB)
{
  // TODO
}

void drawCircle(std::array<unsigned int, 2> origin, unsigned int radius, RGBColour RGB) // low priority
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



int main()
{
  std::cout << "Hello, World!" << std::endl;

  /*
  const ByteMatrix worldNoise = generateNoise(63,127,0.08, 5);
  char c;
  while (true)
  {
    system("cls");
    system("clear"); // Jarrah, this is so it works in linux but the system("cls") might be making ur laptop scream idk. if problems fix! system("cls") is required for it to work in windows
    printCameraViewInTerminal(worldNoise);
    std::cout << "w||a||s||d to move, any other input to exit loop:\n";
    std::cin >> c;
    if (c == 'w')
    {
      cameraPosition[1] -= 1;
    }
    else if (c == 's')
    {
      cameraPosition[1] += 1;
    }
    else if (c == 'a')
    {
      cameraPosition[0] += 1;
    }
    else if (c == 'd')
    {
      cameraPosition[0] -= 1;
    }
    else
    {
      break;
    }
  }
  */



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
    -0.75f, -0.75f, 0.0f, 1.00f, 0.50f, 0.00f,
     0.00f, -0.75f, 0.0f, 1.00f, 0.50f, 0.00f,
    -0.75f,  0.00f, 0.0f, 1.00f, 0.50f, 0.00f,
     0.00f,  0.00f, 0.0f, 1.00f, 0.50f, 0.00f,

     0.75f,  0.75f, 0.0f, 0.50f, 1.00f, 0.00f,
     0.00f,  0.75f, 0.0f, 0.50f, 1.00f, 0.00f,
     0.75f,  0.00f, 0.0f, 0.50f, 1.00f, 0.00f,
     0.00f,  0.00f, 0.0f, 0.50f, 1.00f, 0.00f
  };
  unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3,
    4, 5, 6,
    5, 6, 7
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  //glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



  glViewport(0, 0, 800, 600);


  //std::cout << filepath::exePath() << std::endl;


  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(
      (sin(glfwGetTime()+(3.14f/3*3))+1.0f)/2.0f, // red
      (sin(glfwGetTime()+(3.14f/3*2))+1.0f)/2.0f, // green
      (sin(glfwGetTime()+(3.14f/3*1))+1.0f)/2.0f, // blue
      1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // std::cout << "sin: " << (sin(glfwGetTime())+1.0f)/2.0f << std::endl;
  

    vertices[(6*0)+5] = (sin(glfwGetTime()+(3.14f/3*3))+1.0f)/2.0f;
    vertices[(6*1)+5] = (sin(glfwGetTime()+(3.14f/3*3))+1.0f)/2.0f;
    vertices[(6*2)+5] = (sin(glfwGetTime()+(3.14f/3*3))+1.0f)/2.0f;
    vertices[(6*3)+5] = (sin(glfwGetTime()+(3.14f/3*3))+1.0f)/2.0f;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


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
