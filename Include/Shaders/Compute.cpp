#include "Compute.hpp"

#include <glad/glad.h>

Compute::Compute(const char* path, sf::Vector2u _workSize, sf::Vector2u _screenSize) {
    workSize = _workSize;
    screenSize = _screenSize;

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
    uint32_t shader;

    shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &c_shader_code, NULL);
    glCompileShader(shader);

    // create program
    id = glCreateProgram();
    glAttachShader(id, shader);
    glLinkProgram(id);

    // cleanup
    glDeleteShader(shader);

    glGenTextures(1, &texWorld);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texWorld);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, texWorld, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glGenTextures(1, &texVision);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texVision);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(1, texVision, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);


    glGenTextures(1, &texTransform);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texTransform);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, workSize.x, workSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(2, texTransform, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

Compute::~Compute() {
    glDeleteProgram(id);
    glDeleteTextures(1, &texWorld);
    glDeleteTextures(1, &texVision);
    glDeleteTextures(1, &texTransform);
}

void Compute::use() {
    glUseProgram(id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texWorld);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texVision);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texTransform);
}

void Compute::dispatch(uint32_t local_size_x, uint32_t local_size_y) {
    glDispatchCompute(workSize.x / local_size_x, workSize.y / local_size_y, 1);
}

void Compute::wait() {
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void Compute::set_values(float* values) {
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, values);
}

void Compute::set_transform(float* values) {
    glActiveTexture(GL_TEXTURE2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, workSize.x, workSize.y, 0, GL_RGBA, GL_FLOAT, values);
}

std::vector<float> Compute::get_values() {
    unsigned int collection_size = screenSize.x * screenSize.y;
    std::vector<float> compute_data(collection_size * 4u);

    glActiveTexture(GL_TEXTURE1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, compute_data.data());

    return compute_data;
}



