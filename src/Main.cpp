#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glfw3.h>
#include <cassert>
#include <glad/glad.h>
#include "Shader.h"

int main()
{
    // init glfw
    assert(glfwInit() != GLFW_FALSE);

    // setup glfw window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // create glfw window
    const int32_t c_height{720};
    const int32_t c_width{720};
    GLFWwindow* window {glfwCreateWindow(c_width, c_height, "Ray Tracing In One Weekend", nullptr, nullptr)};
    if (window == NULL)
        return -1;
    glfwMakeContextCurrent(window);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    // create shader
    Shader shader("shaders/shader.hlsl");

     float vertices[] = {
         1.0f,  1.0f, 0.0f,    1.0f, 1.0f, 
         1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,    0.0f, 1.0f
    };

    uint32_t indices[] = { 
        0, 1, 3,  
        1, 2, 3   
    };

    uint32_t vao{};
    uint32_t vbo{};
    uint32_t ibo{};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // create texture data
    int32_t pixels{c_width * c_height * 4};
    float* pixelData{new float[pixels]};
    for (int32_t i{0}; i < pixels; i += 4)
    {
        pixelData[i] = static_cast<float>(rand()) / RAND_MAX;
        pixelData[i + 1] = static_cast<float>(rand()) / RAND_MAX;
        pixelData[i + 2] = static_cast<float>(rand()) / RAND_MAX;
        pixelData[i + 3] = 1.0f;
    }

    // create texture to render to
    uint32_t texture{};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
             c_width, c_height, 0, GL_RGBA, GL_FLOAT, pixelData);

    while (!glfwWindowShouldClose(window))
    {   
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setInt("uTexture", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);
    delete[] pixelData;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteTextures(1, &texture);
    return 0;
}