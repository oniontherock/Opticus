#ifndef __COMPUTE_H__
#define __COMPUTE_H__

#include <SFML/System/Vector2.hpp>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Compute {
public:
    unsigned int id;
    unsigned int out_tex;

    Compute(const char* path, sf::Vector2u size) {
        work_size = size;

        // read in shader code
        std::string compute_code;
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            file.open(path);
            std::stringstream file_stream;
            file_stream << file.rdbuf();
            file.close();

            compute_code = file_stream.str();
        }
        catch (std::ifstream::failure e) {
            std::cerr << "failed to read compute shader file" << std::endl;
        }

        const char* c_shader_code = compute_code.c_str();

        // compile shader
        unsigned int shader;

        shader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(shader, 1, &c_shader_code, NULL);
        glCompileShader(shader);

        // create program
        id = glCreateProgram();
        glAttachShader(id, shader);
        glLinkProgram(id);

        // cleanup
        glDeleteShader(shader);

        // create input/output textures
        glGenTextures(1, &out_tex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, out_tex);

        // turns out we need this. huh.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // create empty texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, work_size.x, work_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindImageTexture(0, out_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    }

    ~Compute() {
        glDeleteProgram(id);
    }

    void use() {
        glUseProgram(id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, out_tex);
    }

    void dispatch() {
        // just keep it simple, 2d work group
        glDispatchCompute(work_size.x / 8, work_size.y / 8, 1);
    }

    void wait() {
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }

    void set_values(float* values) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, work_size.x, work_size.y, 0, GL_RGBA, GL_FLOAT, values);
    }

    std::vector<float> get_values() {
        unsigned int collection_size = work_size.x * work_size.y;
        std::vector<float> compute_data(collection_size * 4u);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, compute_data.data());

        return compute_data;
    }

private:
    sf::Vector2u work_size;
};


#endif