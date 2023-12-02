#pragma once

#include <glm/glm.hpp>

struct LightBase
{
    glm::vec3 colour = {1, 1, 1};
    float ambient_intensity = 0.2f;
    float diffuse_intensity = 0.2f;
    float specular_intensity = 0.2f;
};

struct Attenuation
{
    float constant = 1.0f;
    float linear = 0.045f;
    float exponant = 0.0075f;
};

struct DirectionalLight : public LightBase
{
    glm::vec3 direction = {0, -1, 0};
};

struct PointLight : public LightBase
{
    Attenuation att;
    glm::vec3 position = {0, 0, 0};
};

struct SpotLight : public LightBase
{
    Attenuation att;
    glm::vec3 direction = {0, -1, 0};
    glm::vec3 position = {0, 0, 0};

    float cutoff = 12.5;
};