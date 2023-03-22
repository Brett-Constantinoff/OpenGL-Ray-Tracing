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
#include "Vec3.h"
#include "Ray.h"

Vec3<float> sphereHit(const Vec3<float>& center, float radius, const Ray& ray)
{
    Vec3<float> originToCenter{ ray.m_origin - center };
    float a{ dot<float>(ray.m_direction, ray.m_direction) };
    float b{ 2.0f * dot<float>(originToCenter, ray.m_direction) };
    float c{ dot<float>(originToCenter, originToCenter) - radius * radius };
    float discriminant{ b * b - 4.0f * a * c };

    Vec3<float> dir{ unit<float>(ray.m_direction) };
    float t{ 0.5f * (dir.y() + 1.0f) };

    bool hit{ discriminant > 0 };

    if (hit)
        return Vec3<float>{1.0f, 0.0f, 0.0f};
    else
        return Vec3<float>{ (1.0f - t) * Vec3<float>{1.0f, 1.0f, 1.0f} + t * Vec3<float>{0.5f, 0.7f, 1.0f } };
}

int main()
{
    // init glfw
    assert(glfwInit() != GLFW_FALSE);

    // setup glfw window hints
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // create glfw window
    const float c_aspect{ 16.0f / 9.0f };
    const int32_t c_width{ 1080 };
    const int32_t c_height{ static_cast<int32_t>(c_width / c_aspect) };

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
    int32_t pixels{c_width * c_height * 3};
    float* pixelData{new float[pixels]};

    // camera
    float viewHeight{ 2.0f };
    float viewWidth{ c_aspect * viewHeight };
    float focalLen{ 1.0f };

    Vec3<float> origin{};
    Vec3<float> horizontal{ viewWidth, 0.0f, 0.0f };
    Vec3<float> vertical{ 0.0f, viewHeight, 0.0f };
    Vec3<float> lowerLeft{ origin - horizontal / 2.0f - vertical / 2.0f - Vec3<float>(0.0f, 0.0f, focalLen) };

    for (int32_t y{ c_height - 1 }; y >= 0; y--)
    {
        for (int32_t x{ 0 }; x < c_width; x++)
        {
            int32_t i{ (y * c_width + x) * 3 };

            float u{ static_cast<float>(x) / (c_width - 1) };
            float v{ static_cast<float>(y) / (c_height - 1) };

            Ray ray{ origin, lowerLeft + u * horizontal + v * vertical - origin };

            //check to see if we hit a sphere
            Vec3<float> center{ 0.0f, 0.0f, -1.0f };
            float radius{ 0.5f };

            Vec3<float> color{ sphereHit(center, radius, ray) };

            pixelData[i] = color.r();
            pixelData[i + 1] = color.g();
            pixelData[i + 2] = color.b();
        }
    }

    // create texture to render to
    uint32_t texture{};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
             c_width, c_height, 0, GL_RGB, GL_FLOAT, pixelData);

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