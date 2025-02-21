#include <iostream>
#include <array>
#include <vector>
#include <chrono>
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

int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
