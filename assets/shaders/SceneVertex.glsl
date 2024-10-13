#version 450 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texture_coord;

out vec2 pass_texture_coord;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main() 
{
    pass_texture_coord = in_texture_coord;
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(in_position, 1.0);
}