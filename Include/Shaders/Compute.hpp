#ifndef __COMPUTE_H__
#define __COMPUTE_H__

#include <SFML/System/Vector2.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Compute {
    uint32_t id;
    unsigned int textures[3];
    sf::Vector2u workSize;
    sf::Vector2u screenSize;

    Compute();
    Compute(const char* path, sf::Vector2u _workSize, sf::Vector2u _screenSize);
    ~Compute();
    void terminate();
    void use();
    void dispatch(uint32_t local_size_x, uint32_t local_size_y);
    void wait();
    void set_values(float* values);
    void set_transform(float* values);
    std::vector<float> get_values();

private:
    bool didTerminate = false;
};


#endif