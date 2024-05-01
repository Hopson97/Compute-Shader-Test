#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform readonly image2D in_screen;
layout(rgba32f, binding = 1) uniform writeonly image2D out_screen;

const vec4 DEAD = vec4(0, 0, 0, 0);
const vec4 ALIVE = vec4(1, 1, 1, 1);

int get_pixel_value(ivec2 pixel_coords, ivec2 image_size) 
{
    if (all(greaterThanEqual(pixel_coords, ivec2(0))) && all(lessThan(pixel_coords, image_size))) 
    {
        return int(imageLoad(in_screen, pixel_coords).r);
    } 
    else 
    {
        return 0;
    }
}

void main()
{
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    imageStore(out_screen, pixel_coords, vec4(1,0,1,1));
    ivec2 image_size = imageSize(in_screen);

    vec4 current = imageLoad(in_screen, pixel_coords);
    bool alive = int(current.r) == 1;

    int neighbours = 0;
    for (int y = -1; y <= 1; y++) 
    {
        for (int x = -1; x <= 1; x++) 
        {
            if (x == 0 && y == 0) {
                continue;
            }
            neighbours += get_pixel_value(pixel_coords + ivec2(x, y), image_size);
        }
    }

    if (!alive && neighbours == 3) 
    {
        imageStore(out_screen, pixel_coords, ALIVE);
    }
    else if (alive && (neighbours < 2 || neighbours > 3)) 
    {
        imageStore(out_screen, pixel_coords, DEAD);
    }
    else
    {
        imageStore(out_screen, pixel_coords, current);
    }
}