#version 450

in vec2 pass_texture_coord;
out vec4 out_colour;

uniform sampler2D texture_Colour;

void main()
{
    out_colour = texture(texture_Colour, pass_texture_coord);
    out_colour = vec4(0.5, 0.5, 1.0, 1.0);
}