#include "Shader.h"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../Util.h"

namespace
{
    enum class IVParameter
    {
        CompileStatus = GL_COMPILE_STATUS,
        LinkStatus = GL_LINK_STATUS
    };

    constexpr const char* to_string(IVParameter param)
    {
        switch (param)
        {
            case IVParameter::CompileStatus:
                return "compile";

            case IVParameter::LinkStatus:
                return "link";
        }
        return "ShouldNeverGetHere";
    }

    bool verify_shader(GLuint shader, IVParameter parameter)
    {
        // Verify
        GLint status = 0;
        if (parameter == IVParameter::CompileStatus)
        {
            glGetShaderiv(shader, static_cast<GLenum>(parameter), &status);
        }
        else if (parameter == IVParameter::LinkStatus)
        {
            glGetProgramiv(shader, static_cast<GLenum>(parameter), &status);
        }
        else
        {
            std::cerr << "Unkown verify type for action '" << to_string(parameter) << "'\n";
        }

        if (status == GL_FALSE)
        {
            GLsizei length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            if (parameter == IVParameter::CompileStatus)
            {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            }
            else if (parameter == IVParameter::LinkStatus)
            {
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
            }

            std::cout << "LENGTH " << length << std::endl;
            // std::string buffer(length + 1, ' ');
            std::string buffer(2500, ' ');
            glGetShaderInfoLog(shader, 1024, NULL, buffer.data());
            std::cerr << "Failed to " << to_string(parameter) << " shader :\n" << buffer << std::endl;
            return false;
        }
        return true;
    }

    GLuint compile_shader(const char* source, GLuint shader_type)
    {
        GLuint shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, (const GLchar* const*)&source, nullptr);
        glCompileShader(shader);

        if (!verify_shader(shader, IVParameter::CompileStatus))
        {
            return 0;
        }
        return shader;
    }
} // namespace

Shader::Shader()
{
}

Shader::~Shader()
{
    glDeleteProgram(program_);
}

bool Shader::load_stage(const std::filesystem::path& file_path, ShaderType shader_type)
{
    // Load the files into strings and verify
    auto source = read_file_to_string(file_path);
    if (source.length() == 0)
    {
        return false;
    }

    std::cout << "Compiling " << file_path << ".\n";
    GLuint shader = compile_shader(source.c_str(), static_cast<GLenum>(shader_type));
    if (!shader)
    {
        std::cerr << "Failed to compile shader file " << file_path << ".\n";
        return false;
    }
    stages_.push_back(shader);

    return true;
}

bool Shader::link_shaders()
{
    // Link the shaders together and verify the link status
    program_ = glCreateProgram();
    for (auto stage : stages_)
    {
        glAttachShader(program_, stage);
    }
    glLinkProgram(program_);

    if (!verify_shader(program_, IVParameter::LinkStatus))
    {
        std::cerr << "Failed to link shaders\n";
        return false;
    }
    glValidateProgram(program_);

    int status = 0;
    glGetProgramiv(program_, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << "Failed to validate shader program.\n";
        return false;
    }

    // Delete the temporary shaders
    for (auto& shader : stages_)
    {
        glDeleteShader(shader);
    }
    stages_.clear();
    stages_.shrink_to_fit();

    return true;
}

void Shader::bind() const
{
    glUseProgram(program_);
}

void Shader::set_uniform(const std::string& name, int value)
{
    glProgramUniform1i(program_, get_uniform_location(name), value);
}

void Shader::set_uniform(const std::string& name, float value)
{
    glProgramUniform1f(program_, get_uniform_location(name), value);
}

void Shader::set_uniform(const std::string& name, const glm::vec3& vector)
{
    glProgramUniform3fv(program_, get_uniform_location(name), 1, glm::value_ptr(vector));
}

void Shader::set_uniform(const std::string& name, const glm::vec4& vector)
{
    glProgramUniform4fv(program_, get_uniform_location(name), 1, glm::value_ptr(vector));
}

void Shader::set_uniform(const std::string& name, const glm::mat4& matrix)
{
    glProgramUniformMatrix4fv(program_, get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::bind_uniform_block_index(const std::string& name, GLuint index)
{
    glUniformBlockBinding(program_, glGetUniformBlockIndex(program_, name.c_str()), index);
}

GLint Shader::get_uniform_location(const std::string& name)
{
    auto itr = uniform_locations_.find(name);
    if (itr == uniform_locations_.end())
    {
        auto location = glGetUniformLocation(program_, name.c_str());
        if (location == -1)
        {
            std::cerr << "Cannot find uniform location '" << name << "'\n";
            return 0;
        }
        uniform_locations_.insert({name, location});

        return location;
    }

    return itr->second;
}
