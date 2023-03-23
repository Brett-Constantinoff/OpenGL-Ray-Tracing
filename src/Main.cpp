#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <cassert>
#include "Window.h"
#include "Shader.h"
#include "Vec3.h"
#include "Ray.h"
#include "Sphere.h"

struct Geo
{
    uint32_t m_vbo{};
    uint32_t m_ibo{};
    uint32_t m_vao{};
};

Geo setupGeometry()
{
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

    return {vbo, ibo, vao};
}

Vec3<float> rayColor(Ray& ray, const std::vector<Sphere>& hittables)
{
    HitRecord record{};
    HitRecord recordTemp{};
    bool hitAnything{};
    float closestT{std::numeric_limits<float>::infinity()};
    float tMin{0.0f};

    // loop through all hittable objects and record closest hit
    for (const auto& hittable : hittables)
    {
        if (hittable.hit(ray, tMin, closestT, recordTemp))
        {
            hitAnything = true;
            closestT = record.m_t;
            record = recordTemp;
        }
    }
    
    // colour using normal
    if (hitAnything)
        return 0.5f * (record.m_normal + Vec3<float>(1.0f, 1.0f, 1.0f));
    
    // color the backround (linear interpolation based on y coord for whitish - blue)
    Vec3<float> unitDir{unit<float>(ray.m_direction)};
    float t{0.5f * (unitDir.y() + 1.0f)};
    return (1.0f - t) * Vec3<float>(1.0f, 1.0f, 1.0f) + t * Vec3<float>(0.5f, 0.7f, 1.0f);
}

void renderTexture(const Window& window, float* pixelData, const int32_t pixels)
{
    for (int32_t i{ 0 }; i < pixels; i++)
    {
        pixelData[i] = static_cast<float>(std::rand()) / RAND_MAX;
    }
}

int main()
{
    std::srand(std::time(nullptr));
    
    Window window{ 1080, 16.0f / 9.0f, "Raytracing" };

    // create shader
    Shader shader("shaders/shader.hlsl");

    // get our vao for geometry
    Geo geo{setupGeometry()};

    // create texture to render to
    uint32_t texture{};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
             window.m_width, window.m_height, 0, GL_RGB, GL_FLOAT, nullptr);

    int32_t pixels{ window.m_width * window.m_height * 3 };
    float* pixelData{ new float[pixels] };

    while (!glfwWindowShouldClose(window.m_window))
    {   
        float initialTime{ static_cast<float>(glfwGetTime()) };
        renderTexture(window, pixelData, pixels);
        float endTime{ static_cast<float>(glfwGetTime()) };
        std::cout << "Render Time (ms): " << (endTime - initialTime) * 1000 << "\r";
    
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setInt("uTexture", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, window.m_width, window.m_height, GL_RGB, GL_FLOAT, pixelData);

        glBindVertexArray(geo.m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window.m_window);
        glfwPollEvents();
    }

    delete[] pixelData;
    glDeleteVertexArrays(1, &geo.m_vao);
    glDeleteBuffers(1, &geo.m_vbo);
    glDeleteBuffers(1, &geo.m_ibo);
    glDeleteTextures(1, &texture);
    return 0;
}