#include "RayTracer.h"

RayTracer::RayTracer(std::shared_ptr<Window> window) :
    m_window{window}, m_width{window->m_width}, m_height{window->m_height}, m_shader{"shaders/shader.hlsl"},
    m_aspect{m_window->m_aspect}
{
    m_pixels = m_width * m_height * 3;
    m_pixelData = new float[m_pixels];

	initGeometry();
    initImage();
}

RayTracer::~RayTracer()
{
    delete[] m_pixelData;
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
    glDeleteTextures(1, &m_image);
}

void RayTracer::render()
{
    while (m_window->isOpen())
    {
        float initialTime{ static_cast<float>(glfwGetTime()) };
        renderImage();
        std::cout << "Render Time (ms): " << (static_cast<float>(glfwGetTime()) - initialTime) * 1000.0f << "\r";

        glClear(GL_COLOR_BUFFER_BIT);

        m_shader.use();
        m_shader.setInt("uTexture", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_image);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_pixelData);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        m_window->pollEvents();
    }
}

glm::vec3 RayTracer::colorPixxel(float u, float v)
{
    m_rayOrg.x = 0.0f;
    m_rayOrg.y = 0.0f;
    m_rayOrg.z = 2.0f;

    m_rayDir.x = u * m_aspect;
    m_rayDir.y = v;
    m_rayDir.z = -1.0f;


    float r{ 0.5f };

    float a{ glm::dot(m_rayDir, m_rayDir)};
    float b{ 2.0f * glm::dot(m_rayOrg, m_rayDir) };
    float c{glm::dot(m_rayOrg, m_rayOrg) - r * r };

    float discr{ b * b - 4 * a * c };

    glm::vec3 color{};
    if (discr >= 0)
    {
       color = glm::vec3{1.0f, 0.0f, 1.0f};
    }
    else
    {
        color = glm::vec3{ 0.0f, 0.0f, 0.0f };
    }
    return color;
}

void RayTracer::renderImage()
{
    for (int32_t y{ m_height - 1 }; y >= 0; y--)
    {
        for (int32_t x{ 0 }; x < m_width; x++)
        {
            int32_t i{ (y * m_width + x) * 3 };

            float u{ static_cast<float>(x) / (m_width - 1) };
            float v{ static_cast<float>(y) / (m_height - 1) };

            // remap to -1 1
            u = u * 2.0f - 1.0f;
            v = v * 2.0f - 1.0f;

            glm::vec3 color{ colorPixxel(u, v) };

            m_pixelData[i] = color.r;
            m_pixelData[i + 1] = color.g;
            m_pixelData[i + 2] = color.b;
        }
    }
}

void RayTracer::initGeometry()
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
    
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void RayTracer::initImage()
{
    glGenTextures(1, &m_image);
    glBindTexture(GL_TEXTURE_2D, m_image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);
}
	