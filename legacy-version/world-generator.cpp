#include <cmath>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <thread>

#include <iostream>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

int width,height;
std::vector<std::vector<float>> points;
std::vector<int> coords = {0,0};

float waterLevel = 0.4;

uint64_t timeMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
int random(int high) {
    return ((rand()+timeMillisec()) % high);
}

std::vector<std::vector<float>> placePoints(std::vector<std::vector<float>> grid, double density) {
    for (int y = 0; y < grid.size(); y++) { // place the seeds
        for (int x = 0; x < grid[y].size(); x++) {
            if ((double)random(10000) / 10000 < density) {
                grid[y][x] = 1.0;
            } else {
                grid[y][x] = 0.0;
            }
        }
    }
    return grid;
}

std::vector<std::vector<float>> generateNoise(std::vector<std::vector<float>> grid, int passes = 3, int blur = 4) {

    std::vector<std::vector<float>> tempGrid(grid.size(), std::vector<float>(grid[0].size(), 0.0));

    for (int pass = 0; pass < passes; pass++) { // spread the noise
        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid[0].size(); x++) {
                if (grid[y][x] != 0) {
                    if (y > 0) {
                        tempGrid[y-1][x] += (grid[y-1][x] + grid[y][x])/4 + (float)random(1000)/1000 - 0.5;
                    } 
                    if (y+1 < height) {
                        tempGrid[y+1][x] += (grid[y+1][x] + grid[y][x])/4 + (float)random(1000)/1000 - 0.5;
                    }
                    if (x > 0) {
                        tempGrid[y][x-1] += (grid[y][x-1] + grid[y][x])/4 + (float)random(1000)/1000 - 0.5;
                    }
                    if (x+1 < width) {
                        tempGrid[y][x+1] += (grid[y][x+1] + grid[y][x])/4 + (float)random(1000)/1000 - 0.5;
                    }
                    tempGrid[y][x] += grid[y][x];
                }
            }
        }
        grid = tempGrid;
        tempGrid = std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0));
    }
    // blur the result [in 2x2 blocks]
    int totalPoints;
    float totalValue;
    float average;
    for (int i = 0; i < blur; i++) {
        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid[0].size(); x++) {
                // find the average of the block
                totalPoints = 1;
                totalValue = grid[y][x];
                if (y+1 < height) {
                    totalValue += grid[y+1][x];
                    totalPoints += 1;
                }
                if (x+1 < width) {
                    totalValue += grid[y][x+1];
                    totalPoints += 1;
                }
                if (y+1 < height && x+1 < width) {
                    totalValue += grid[y+1][x+1];
                    totalPoints += 1;
                }
                // apply the average to the points
                average = totalValue / totalPoints; 
                grid[y][x] = average;
                if (y+1 < height) {
                    grid[y+1][x] = average;
                }
                if (x+1 < width) {
                    grid[y][x+1] = average;
                }
                if (y+1 < height && x+1 < width) {
                    grid[y+1][x+1] = average;
                }
            }
        }
    }
    return grid;
}

void move() {
    std::cout << 's' << std::endl;
    int c = 0;
    switch((c=getch())) {
        case KEY_UP:
            std::cout << 'u' << std::endl;
            coords[1] -= 1;
            break;
        case KEY_DOWN:
            std::cout << 'd' << std::endl;
            coords[1] += 1;
            break;
        case KEY_LEFT:
            std::cout << 'l' << std::endl;
            coords[0] -= 1;
            break;
        case KEY_RIGHT:
            std::cout << 'r' << std::endl;
            coords[0] += 1;
            break;
    }
    std::cout << coords[0] << ',' << coords[1] << std::endl;
}

void SFMLDisplay(int windowWidth, int windowHeight, bool portion = false) {
    sf::RenderWindow window(sf::VideoMode(windowHeight,windowWidth), "New World");

    sf::RectangleShape sliderPanel(sf::Vector2f(windowWidth/4,windowHeight/24));
    sliderPanel.setPosition(0,0);
    sliderPanel.setFillColor(sf::Color(180,180,180));

    sf::RectangleShape sliderHandle(sf::Vector2f(windowWidth/60,windowHeight/30));
    sliderHandle.setPosition(waterLevel*((windowWidth*7)/30),windowHeight/240);
    sliderHandle.setFillColor(sf::Color(120,120,120));

    bool dragging = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // close window when user click the cross
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) { // start dragging
                dragging = (sliderHandle.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x,event.mouseButton.y)));
            } else if (event.type == sf::Event::MouseButtonReleased) { // stop dragging
                dragging = false;
            } else if (event.type == sf::Event::MouseMoved && dragging) { // drag
                if (event.mouseMove.x <= (windowWidth*7)/30 && event.mouseMove.x >= windowWidth/60) {
                    sliderHandle.setPosition(event.mouseMove.x,windowHeight/240);
                } else if (event.mouseMove.x >= (windowWidth*7)/30) {
                    sliderHandle.setPosition((windowWidth*7)/30,windowHeight/240);
                } else {
                    sliderHandle.setPosition(windowWidth/60,windowHeight/240);
                }
                waterLevel = (float)sliderHandle.getPosition().x / ((windowWidth*7)/30);
            }
        }
        window.clear();
        //
        if (!portion) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    sf::RectangleShape pixel(sf::Vector2f(windowWidth/width,windowHeight/height));
                    pixel.setPosition(x*(windowWidth/width),y*(windowHeight/height));
                    if (points[y][x] > 1) {points[y][x] = 1.0;} // ensure the point isn't too bright
                    if (points[y][x] < 0) {points[y][x] = 0.0;} // ensure the point isn't too dark

                    if (points[y][x] < waterLevel) { // water
                        pixel.setFillColor(sf::Color(0,round(points[y][x]*64),100+round(points[y][x]*255)));
                    } else if (points[y][x] < waterLevel+0.05) { // sand
                        pixel.setFillColor(sf::Color(255,225,64));
                    } else { // land
                        pixel.setFillColor(sf::Color(round(points[y][x]*80),round(points[y][x]*255),round(points[y][x]*64)));
                    }

                    window.draw(pixel); // todo: draw pixels after all pixels have been defined
                }
            }
        } else {
            int portionSize = 100;
            std::vector<int> relativeCoords = coords;

            for (int y = 0; y < portionSize; y++) {
                relativeCoords[1] = y + coords[1];
                for (int x = 0; x < portionSize; x++) {
                    relativeCoords[0] = x + coords[0];

                    if (relativeCoords[0] > points[0].size() || relativeCoords[0] == 0 || relativeCoords[1] > points.size() || relativeCoords[1] == 0) {
                        sf::RectangleShape pixel(sf::Vector2f(windowWidth/width,windowHeight/height));
                        pixel.setPosition(relativeCoords[0]*(windowWidth/width),relativeCoords[1]*(windowHeight/height));
                        pixel.setFillColor(sf::Color(100,0,0));
                        window.draw(pixel);
                        continue;
                    }

                    sf::RectangleShape pixel(sf::Vector2f(windowWidth/portionSize,windowHeight/portionSize));
                    pixel.setPosition(relativeCoords[0]*(windowWidth/portionSize),relativeCoords[1]*(windowHeight/portionSize));

                    if (points[relativeCoords[0]][relativeCoords[1]] > 1) {points[relativeCoords[0]][relativeCoords[1]] = 1.0;} // ensure the point isn't too bright
                    if (points[relativeCoords[0]][relativeCoords[1]] < 0) {points[relativeCoords[0]][relativeCoords[1]] = 0.0;} // ensure the point isn't too dark

                    if (points[relativeCoords[1]][relativeCoords[0]] < waterLevel) { // water
                        pixel.setFillColor(sf::Color(0,round(points[relativeCoords[1]][relativeCoords[0]]*64),100+round(points[relativeCoords[1]][relativeCoords[0]]*255)));

                    } else if (points[relativeCoords[1]][relativeCoords[0]] < waterLevel+0.05) { // sand
                        pixel.setFillColor(sf::Color(255,225,64));

                    } else { // land
                        pixel.setFillColor(sf::Color(round(points[relativeCoords[1]][relativeCoords[0]]*80),round(points[relativeCoords[1]][relativeCoords[0]]*255),round(points[relativeCoords[1]][relativeCoords[0]]*64)));
                    }

                    window.draw(pixel); 
                }
            }
        }

        window.draw(sliderPanel);
        window.draw(sliderHandle);
        

        window.display();
    }
}

int windowWidth;
int windowHeight;


void outputThread() {
    SFMLDisplay(windowWidth,windowHeight,true);
}
void inputThread() {
    while(true) {
        move();
    }
}

int main() {
   width = 400;
   height = 400;
   windowWidth = 1600;
   windowHeight = 1600;

    std::vector<std::vector<std::vector<float>>> grids;
    points = std::vector<std::vector<float>>(height, std::vector<float>(width,0.0));
    std::vector<std::vector<float>> seeds;

    
    for (int i = 1; i < 2; i++) {
        seeds = placePoints(points,(double)i*3/32384);
        grids.push_back(generateNoise(seeds,i*4,100));
        auto& currentGrid = grids.back();
        std::cout << i << std::endl;
         for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                points[y][x] += currentGrid[y][x];
            }
        }
    }
    
    
    seeds = placePoints(points,(double)0.002);
    points = generateNoise(seeds,5,100);
    
    std::thread threadOne(outputThread);
    std::thread threadTwo(inputThread);

    threadOne.join();

    return 0;
}
// todo: view only a portion of the world and navigate it with arrow keys
// todo: generate world as you go
// todo: when user lets go of slider ss the world and render it to a texture to save resources 
// todo: move brightness clamps to the noise generation function

// compile command: g++ world-generator.cpp -lsfml-graphics -lsfml-window -lsfml-system -o world-generator