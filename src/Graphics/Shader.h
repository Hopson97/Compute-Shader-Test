#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

class Shader
{
  public:
    Shader() = default;
    Shader(Shader&& other) noexcept = delete;
    Shader(const Shader& other) = delete;
    Shader& operator=(Shader&& other) noexcept = delete;
    Shader& operator=(const Shader& other) = delete;
    ~Shader();

    bool load_from_file(const std::filesystem::path& vertex_file_path,
                        const std::filesystem::path& fragment_file_path);

    bool load_compute(const std::filesystem::path& compute_file_path);

    void bind() const;

    void set_uniform(const std::string& name, int value);
    void set_uniform(const std::string& name, float value);
    void set_uniform(const std::string& name, const glm::vec3& vect);
    void set_uniform(const std::string& name, const glm::vec4& vect);
    void set_uniform(const std::string& name, const glm::mat4& matrix);

    void bind_uniform_block_index(const std::string& name, GLuint index);



  private:
    GLint get_uniform_location(const std::string& name);

  private:
    std::unordered_map<std::string, GLint> uniform_locations_;
    GLuint program_ = 0;
};