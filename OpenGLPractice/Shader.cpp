#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"




Shader::Shader(const std::string& filePath)
	:m_FilePath(filePath), m_RendererID(0)
{
    //makes some shader code from the file at this location
    ShaderProgramSource source = parseShader(filePath);
    //compiles the shader and returns the location on the gpu
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

//allows us to parse in shader from files instread of string literals
ShaderProgramSource Shader::parseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }return { ss[0].str(), ss[1].str() };
}

/*compile a shader (gpu program) from a string*/
unsigned int Shader::CompileShader(unsigned int type, const std::string source) {
    //creates a shader of specified type
    unsigned int id = glCreateShader(type);
    //stores the source code as a string
    const char* src = source.c_str();
    //load source code into openGL and compiles it
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    //makes sure code compiled correctly
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    //if compile error, print our the error
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "failed to compile \n" << message << "\n";
        GLCall(glDeleteShader(id));
        return 0;
    }
    //else return compiled code location
    return id;
}

//create the shaders from strings
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    //makes a new gpu program, and both shaders compiled code
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attatch shaders to program and makes sure it is valid
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    //delete the shaders since they are incorporated into the program and return program
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return(program);
}


void Shader::Bind() const
{
    //tells openGL to use this shader
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    //tells openGL to use this shader
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{

    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));

}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "warning: uniform '" << name << "' doesnt exist";

    m_UniformLocationCache[name] = location;
    return location;
}