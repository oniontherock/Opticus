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
    uint32_t out_tex;
    sf::Vector2u work_size;

    Compute(const char* path, sf::Vector2u size);
    ~Compute();
    void use();
    void dispatch(uint32_t local_size_x, uint32_t local_size_y);
    void wait();
    void set_values(float* values);
    std::vector<float> get_values();
};


#endif