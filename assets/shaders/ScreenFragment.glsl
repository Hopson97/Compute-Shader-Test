#version 450

in vec2 pass_texture_coord;
out vec4 out_colour;

uniform sampler2D image;

void main()
{
    out_colour = texture(image, pass_texture_coord);
}