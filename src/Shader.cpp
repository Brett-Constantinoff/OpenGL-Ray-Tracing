#include "Shader.h"


Shader::Shader(const std::string& filepath)
{
    m_ID = glCreateProgram(); //creates current shader program

    Shader::parseShader(filepath);
    uint32_t vShader = Shader::compileShader(m_shaderSource.vertexSource, GL_VERTEX_SHADER); //compiles vertex source
    uint32_t fShader = Shader::compileShader(m_shaderSource.fragmentSource, GL_FRAGMENT_SHADER); //compiles fragment source

    //attaches shaders to program
    glAttachShader(m_ID, vShader);
    glAttachShader(m_ID, fShader);
    //links the shader program
    glLinkProgram(m_ID);
    //ensures successful linking
    glValidateProgram(m_ID);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::use(void)
{
    glUseProgram(m_ID);
}

void Shader::disuse(void)
{
    glUseProgram(0);
}

int32_t Shader::getId(void)
{
    return m_ID;
}

int32_t Shader::compileShader(const std::string& source, int32_t type)
{
    const char* src = source.c_str();

    int32_t shadermID = glCreateShader(type);
    glShaderSource(shadermID, 1, &src, nullptr);
    glCompileShader(shadermID);

    int32_t success;
    GLchar infoLog[512];
    glGetShaderiv(shadermID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shadermID, 512, NULL, infoLog);
        glDeleteShader(shadermID);
        throw std::runtime_error{ "ERROR:SHADER::" + std::to_string(type) + "::COMPILATION_FAILED::" + infoLog };
    }
    return shadermID;
}

void Shader::parseShader(const std::string& filePath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    std::ifstream stream(filePath);
    if (!stream)
        throw::std::runtime_error{ "ERROR::CANNOT FIND SHADER FILE " + filePath };

    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertexShader") != std::string::npos)
                type = ShaderType::VERTEX;
            
            else if (line.find("fragmentShader") != std::string::npos)
                type = ShaderType::FRAGMENT;
            
        }
        else
            ss[(int32_t)type] << line << '\n';
    }
    m_shaderSource.vertexSource = ss[0].str();
    m_shaderSource.fragmentSource = ss[1].str();
}

int32_t Shader::getUniform(const std::string& name)
{
    if (m_uniformCache.find(name.c_str()) != m_uniformCache.end())
        return m_uniformCache[name.c_str()];
    

    int32_t location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
        throw::std::runtime_error{ "ERROR::CANNOT FIND UNIFORM " + name };

    m_uniformCache[name.c_str()] = location;
    return location;
}

void Shader::setInt(const std::string& name, int32_t uniform)
{
    int32_t location = getUniform(name);
    if (location != -1)
        glUniform1i(location, uniform);
    
}

void Shader::setFloat(const std::string& name, float uniform)
{
    int32_t location = getUniform(name);
    if (location != -1)
        glUniform1f(location, uniform);
}

