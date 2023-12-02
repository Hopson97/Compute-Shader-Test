#include "Shader.h"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../Util.h"

namespace
{
    bool verify_shader(GLuint shader, GLuint status_enum, std::string_view action)
    {
        // Verify
        GLint status = 0;
        if (status_enum == GL_COMPILE_STATUS)
        {
            glGetShaderiv(shader, status_enum, &status);
        }
        else if (status_enum == GL_LINK_STATUS)
        {
            glGetProgramiv(shader, status_enum, &status);
        }
        else
        {
            std::cerr << "Unkown verify type for action '" << action << "'\n";
        }

        if (status == GL_FALSE)
        {
            GLsizei length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            if (status_enum == GL_COMPILE_STATUS)
            {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            }
            else if (status_enum == GL_LINK_STATUS)
            {
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
            }
            std::cout << "LENGTH " << length << std::endl;
            //std::string buffer(length + 1, ' ');
            std::string buffer(2500, ' ');
            glGetShaderInfoLog(shader, 1024, NULL, buffer.data());
            std::cerr << "Failed to " << action << " shader :\n" << buffer << std::endl;
            return false;
        }
        return true;
    }

    GLuint compile_shader(const char* source, GLuint shader_type)
    {
        //  Create and compile
        GLuint shader = glCreateShader(shader_type);

        glShaderSource(shader, 1, (const GLchar* const*)&source, nullptr);
        glCompileShader(shader);

        // Verify
        if (!verify_shader(shader, GL_COMPILE_STATUS, "compile"))
        {
            return 0;
        }
        return shader;
    }
} // namespace

Shader::~Shader()
{
    glDeleteProgram(program_);
}

bool Shader::load_from_file(const std::filesystem::path& vertex_file_path,
                            const std::filesystem::path& fragment_file_path)
{
    // Load the files into strings and verify
    auto vertex_file_source = read_file_to_string(vertex_file_path);
    auto fragment_file_source = read_file_to_string(fragment_file_path);
    if (vertex_file_source.length() == 0 || fragment_file_source.length() == 0)
    {
        return false;
    }

    // Compile the vertex shader
    std::cout << "Compiling " << vertex_file_path << ".\n";
    auto vertex_shader = compile_shader(vertex_file_source.c_str(), GL_VERTEX_SHADER);
    if (!vertex_shader)
    {
        std::cerr << "Failed to compile vertex shader file " << vertex_file_path << ".\n";
        return false;
    }

    // Compile the fragment shader
    std::cout << "Compiling " << fragment_file_path << ".\n";
    auto fragment_shader = compile_shader(fragment_file_source.c_str(), GL_FRAGMENT_SHADER);
    if (!fragment_shader)
    {
        std::cerr << "Failed to compile fragment shader file " << fragment_file_path << ".\n";
        return false;
    }

    // Link the shaders together and verify the link status
    program_ = glCreateProgram();
    glAttachShader(program_, vertex_shader);
    glAttachShader(program_, fragment_shader);
    glLinkProgram(program_);

    if (!verify_shader(program_, GL_LINK_STATUS, "link"))
    {
        std::cerr << "Failed to link" << vertex_file_path << " and " << fragment_file_path
                  << ".\n";
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
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return true;
}

bool Shader::load_compute(const std::filesystem::path& compute_file_path)
{
    // Load the files into strings and verify
    auto compute_file_source = read_file_to_string(compute_file_path);
    if (compute_file_source.length() == 0)
    {
        return false;
    }

    // Compile the vertex shader
    std::cout << "Compiling " << compute_file_path << ".\n";
    auto compute_shader = compile_shader(compute_file_source.c_str(), GL_COMPUTE_SHADER);
    if (!compute_shader)
    {
        std::cerr << "Failed to compile compute shader file " << compute_file_path << ".\n";
        return false;
    }

    glAttachShader(program_, compute_shader);
    glLinkProgram(program_);

    if (!verify_shader(program_, GL_LINK_STATUS, "link"))
    {
        std::cerr << "Failed to link" << compute_file_path << '\n';
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
    glDeleteShader(compute_shader);
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

void Shader::set_uniform(const std::string& name, const glm::vec3& vect)
{
    glProgramUniform3fv(program_, get_uniform_location(name), 1, glm::value_ptr(vect));
}

void Shader::set_uniform(const std::string& name, const glm::vec4& vect)
{
    glProgramUniform4fv(program_, get_uniform_location(name), 1, glm::value_ptr(vect));
}

void Shader::set_uniform(const std::string& name, const glm::mat4& matrix)
{
    glProgramUniformMatrix4fv(program_, get_uniform_location(name), 1, GL_FALSE,
                              glm::value_ptr(matrix));
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
