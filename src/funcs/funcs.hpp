#ifndef FUNCS
#define FUNCS

#include <vector>
#include <chrono>
#include <math.h>
#include <array>
#include <iostream>
using ByteMatrix = std::vector<std::vector<unsigned char>>;



namespace misc {

uint64_t getTimeMicrosec()
{
  using namespace std::chrono;
  return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
}



unsigned int RNG(unsigned int high)
{
  unsigned int numOfRNGCalls = 1;
  unsigned int seed = misc::getTimeMicrosec() * numOfRNGCalls;
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



void printByteMatrix(ByteMatrix matrix) {
  for(auto& line : matrix) {
    for (auto num : line) {
      if (num < 9) std::cout << (int)num+1 << "   ";
      else if (num < 99) std::cout << (int)num+1 << "  ";
      else std::cout << (int)num+1 << ' ';
    }
    std::cout << std::endl;
  }
}

}



namespace noiseMap {

class noiseMap {
public:
  // Default constructor
  noiseMap() : width(0), height(0), density(0), smoothness(0), viewRadius(0) {}

  // Constructor with initial values
  noiseMap(unsigned int inputWidth, unsigned int inputHeight,
    float inputDensity, unsigned char inputSmoothness, unsigned int inputViewRadius) {

    width = inputWidth;
    height = inputHeight;
    density = inputDensity;
    smoothness = inputSmoothness;
    viewRadius = inputViewRadius;


    initMap();
    initNoise();
  }

  std::array<long, 2> cameraPosCentre = {0,0};



  ByteMatrix getMapNoise() {
    return mapNoise;
  }

  ByteMatrix getMapVisible() {
    ByteMatrix tmpMap(viewRadius*2, std::vector<unsigned char>(viewRadius*2, 0));
    for (int i = 0; i < viewRadius*2; i++) {
      for (int j = 0; j < viewRadius*2; j++) {
        if (
          (i+cameraPosCentre[1]-viewRadius) < 0 ||
          (i+cameraPosCentre[1]-viewRadius) >= mapNoise.size()
        ) {
          tmpMap[i][j] = 0x00;
        } else {
          if (
            (j+cameraPosCentre[0]-viewRadius) < 0 ||
            (j+cameraPosCentre[0]-viewRadius) > mapNoise[i].size()
          ) {
            tmpMap[i][j] = 0x00;
          } else {
            tmpMap[i][j] = mapNoise[i+cameraPosCentre[1]-viewRadius][j+cameraPosCentre[0]-viewRadius];
          }
        }
      }
    }
    return tmpMap;
  }



private:
  unsigned int width;
  unsigned int height;
  float density; /*0 -> 1; allblack->all white*/
  unsigned char smoothness;
  unsigned int viewRadius;

  ByteMatrix mapNoise;


  void initMap() {
    ByteMatrix tmpMap(height, std::vector<unsigned char>(width, 0));
    mapNoise = tmpMap;
  }

  void initNoise() {
    //std::cout << "width == " << width << std::endl;
    //std::cout << "height == " << height << std::endl;
    //std::cout << "density == " << density << std::endl;
    // place bright spots on Map
    for (unsigned long i = 0; i < density*width*height; i++) {
      mapNoise[misc::RNG(height)][misc::RNG(width)] = 0xFF;
    }

    // Apply blur
    float brightnessWeight = 0.0f; // How much pixels that make the blur brighter are favoured // TODO
    unsigned short localBrightnessSum;

    while (smoothness > 1) {
      // Blur horizontally //
      for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < std::ceil(width/smoothness); x++) {
          for (unsigned int i = 0; i < smoothness; i++) {
            if (x*smoothness+i > width) {
              localBrightnessSum += localBrightnessSum / i; // To prevent pixels being too dim after the average is set
              break; // Slightly hacky but works
            }
            localBrightnessSum += mapNoise[y][x*smoothness+i];
          }
          for (unsigned int i = 0; i < smoothness; i++) {
            mapNoise[y][x*smoothness+i] = localBrightnessSum / smoothness;
          }
          localBrightnessSum = 0;
        }
      }

      // Blur vertically //
      for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < std::ceil(height/smoothness); y++) {
          for (unsigned int i = 0; i < smoothness; i++) {
            if (y*smoothness+i > height) {
              localBrightnessSum += localBrightnessSum / i; //
              break; //
            }
            localBrightnessSum += mapNoise[y*smoothness+i][x];
          }
          for (unsigned int i = 0; i < smoothness; i++) {
            mapNoise[y*smoothness+i][x] = localBrightnessSum / smoothness;
          }
          localBrightnessSum = 0;
        }
      }

      smoothness -= 1;
    }
    // END of blur code //
  }

};

}



#endif
