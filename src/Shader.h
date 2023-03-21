#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <glad/glad.h>


struct ShaderSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
public:
    Shader(const std::string& filepath);
    ~Shader();
    int32_t getId(void);
    void setInt(const std::string& name, int32_t uniform);
    void setFloat(const std::string& name, float unifrom);
    void use(void);
    void disuse(void);

private:
    void parseShader(const std::string& filePath);
    int32_t compileShader(const std::string& source, int32_t type);
    int32_t getUniform(const std::string& name);

private:
    int32_t m_ID;
    std::unordered_map<const char*, int32_t> m_uniformCache;
    ShaderSource m_shaderSource;
};
