// https://learnopengl.com/Getting-started/Hello-Window

#include <cmath>
#include <iostream>
#include <array>
#include <vector>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glThings/shader_s.hpp"
#include "glThings/GLManager.hpp"
#include "funcs/funcs.hpp"
//#include "currentDir.hpp"

using ByteMatrix = std::vector<std::vector<unsigned char>>;
using RGBColour = std::array<unsigned char, 3>;

const float pi = 3.14159265;
const float e = 2.718281828;
const float pie = pi + e; // I *promise* to use this!

std::array<unsigned int, 2> windowDimensions;
std::array<unsigned int, 2> worldDimensions;





// MOVE TO funcs/funcs.hpp //
/*
void printCameraViewInTerminal(const ByteMatrix matrix)
{
}
*/





// Biome Classification stuff
RGBColour colourPixelByBiome(unsigned char pixelBrightness) // TODO: Improve func name
{
  if (pixelBrightness < 0x10) return {0, 80, 100}; // Water
  if (pixelBrightness < 0x20) return {100, 20, 0}; // Beach
  return {0, 120, 200}; // Land
}





void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

std::array<long, 2> processInput(GLFWwindow *window, std::array<long, 2> cameraPosition) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cameraPosition[1] -= 1;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cameraPosition[0] -= 1;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cameraPosition[1] += 1;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cameraPosition[0] += 1;
  }
  return cameraPosition;
}

int main() {
  std::cout << "Hello, World!" << std::endl;











  // initialise GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  // initialise GLFW window
  GLFWwindow *window = glfwCreateWindow(500, 500, "world-generator", NULL, NULL);
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
  unsigned int circleVAO, rectangleVAO, circleVBO, rectangleVBO;





  glGenVertexArrays(1, &circleVAO);
  glGenBuffers(1, &circleVBO);

  glGenVertexArrays(1, &rectangleVAO);
  glGenBuffers(1, &rectangleVBO);





  window::polygon rectanglePoly(0);

  shapes::create::rectangle(&rectanglePoly,
    window::position(-0.50f, -0.50f, 0.00f, 1.00f),
    window::position(-0.75f, -0.75f, 0.00f, 1.00f),
    window::colour(0.20f, 0.40f, 0.80f, 1.00f)
  );

  float* rectangleVerts = rectanglePoly.getAllData();





  window::polygon circlePoly(0);

  shapes::create::circle(&circlePoly, 0.50f, 0.50f, 0.25f, 45.00f, 360.00f-45.00f, 0.10f, true);

  float* circleVerts = circlePoly.getAllData();





  glBindVertexArray(circleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * circlePoly.getSize() * 8, circleVerts, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float)));
  glEnableVertexAttribArray(1);





  glBindVertexArray(rectangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rectanglePoly.getSize() * 8, rectangleVerts, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float)));
  glEnableVertexAttribArray(1);



  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  std::cout << "circlePoly.getSize(): " << circlePoly.getSize() << std::endl;
  std::cout << "rectanglePoly.getSize(): " << rectanglePoly.getSize() << std::endl;

  // std::cout << filepath::exePath() << std::endl;

  noiseMap::noiseMap worldNoise(63, 127, 0.25, 5, 12);
  //misc::printByteMatrix(worldNoise.getMapVisible());

  int counter = 0;
  uint64_t lastCheck = 0;
  while (!glfwWindowShouldClose(window)) {
    // Count Frames per Second
    if (false) {
      if ((lastCheck + (1*1000*1000)) < misc::getTimeMicrosec()) {
        //std::cout << "frames this second: " << counter << std::endl;
        counter = 0;
        lastCheck = misc::getTimeMicrosec();
      } else {
        counter++;
      }
    }



#ifdef _WIN32
    system("cls");
#elif __linux__
    system("clear");
#endif
    //std::cout << "worldNoise.getMapVisible()[0][0] == " << worldNoise.getMapVisible()[0][0] << std::endl;
    misc::printByteMatrix(worldNoise.getMapVisible());
    std::cout << "w||a||s||d to move, any other input to exit loop:\n";



    worldNoise.cameraPosCentre = processInput(window, worldNoise.cameraPosCentre);
    glClearColor(
      /*
      (sin(glfwGetTime() + (3.14f / 3 * 3)) + 1.0f) / 2.0f, // red
      (sin(glfwGetTime() + (3.14f / 3 * 2)) + 1.0f) / 2.0f, // green
      (sin(glfwGetTime() + (3.14f / 3 * 1)) + 1.0f) / 2.0f, // blue
      */
      0.80f, 0.40, 0.20, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);










    // draw our first triangle
    ourShader.use();

    glBindVertexArray(circleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, circlePoly.getSize());

    glBindVertexArray(rectangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, rectanglePoly.getSize());

    // Modes (Swap out the first object given to glDrawElements
    /*
    * GL_POINTS
    * GL_LINES
    * GL_LINE_STRIP
    * GL_SINE_LOOP
    * GL_TRIANGLES
    * GL_TRIANGLE_STRIP
    * GL_TRIANGLE_FAN
    * autocomplete says there are others as well
    */
    glfwSwapBuffers(window);
    glfwPollEvents();
  }










  glDeleteVertexArrays(1, &circleVAO);
  glDeleteVertexArrays(1, &rectangleVAO);
  glDeleteBuffers(1, &circleVBO);
  glDeleteBuffers(1, &rectangleVBO);
  glfwTerminate();
  return 0;
}
