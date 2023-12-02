#pragma once


#include "Lights.h"

struct Settings
{
    Settings()
    {
        dir_light.direction = {0.3f, -8.0f, 0.3f};
        dir_light.ambient_intensity = 0.02f;
        dir_light.diffuse_intensity = 0.02f;
        dir_light.specular_intensity = 0.0f;

        point_light.ambient_intensity = 0.3f;
        point_light.diffuse_intensity = 1.0f;
        point_light.specular_intensity = 1.0f;
        point_light.att.constant = 1.0f;
        point_light.att.linear = 0.045f;
        point_light.att.exponant = 0.0075f;

        spot_light.ambient_intensity = 0.012f;
        spot_light.diffuse_intensity = 0.35f;
        spot_light.specular_intensity = 1.0f;
        spot_light.att.constant = 0.2f;
        spot_light.att.linear = 0.016f;
        spot_light.att.exponant = 0.003f;
    }
    DirectionalLight dir_light;
    PointLight point_light;
    SpotLight spot_light;

    bool wireframe = false;
    float material_shine = 32.0f;

    bool grass = true;
};