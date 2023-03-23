#pragma once

#include "Window.h"
#include "Shader.h"
#include <stdint.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

class RayTracer
{
public:
	//empty for now
	RayTracer(std::shared_ptr<Window> window);
	~RayTracer();

	void render();

private:
	glm::vec3 colorPixxel(float u, float v);
	void renderImage();

	void initGeometry();
	void initImage();

	

private:
	std::shared_ptr<Window> m_window;

	Shader m_shader;

	int32_t m_width{};
	int32_t m_height{};
	float m_aspect{};

	uint32_t m_vao{};
	uint32_t m_vbo{};
	uint32_t m_ibo{};
	uint32_t m_image{};

	float* m_pixelData{};
	int32_t m_pixels{};

	glm::vec3 m_rayOrg{};
	glm::vec3 m_rayDir{};
};

