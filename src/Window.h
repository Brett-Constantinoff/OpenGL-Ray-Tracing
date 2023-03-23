#pragma once
#include <glfw3.h>
#include "glad/glad.h"
#include <cassert>
#include <string>

struct Window
{
	Window(int32_t width, float aspect, const std::string& title) :
		m_height{ }, m_width{ width }, m_aspect{aspect}, m_window{}
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
        m_height = static_cast<int32_t>(m_width / m_aspect);
        m_window = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
        
        assert(m_window != NULL);
        glfwMakeContextCurrent(m_window);

        // init glad
        assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0);
	}

    ~Window()
    {
        glfwTerminate();
        glfwDestroyWindow(m_window);
    }

    bool isOpen()
    {
        return !glfwWindowShouldClose(m_window);
    }

    void pollEvents()
    {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }


    GLFWwindow* m_window;
	int32_t m_height;
	int32_t m_width;
    float m_aspect;
};