#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform writeonly image2D out_image;

uniform mat4 inv_projection;
uniform mat4 inv_view;

uniform vec3 position;

struct Ray 
{
	vec3 origin;
	vec3 direction;
};

struct Cube 
{
	vec3 origin;
	vec3 size;
};


bool interesect_cube(Ray ray, Cube cube) 
{
    vec3 t_min = (cube.origin - ray.origin) / ray.direction;
    vec3 t_max = ((cube.origin + cube.size) - ray.origin) / ray.direction;
    
    vec3 t1 = min(t_min, t_max);
    vec3 t2 = max(t_min, t_max);
    
    float t_near = max(max(t1.x, t1.y), t1.z);
    float t_far = min(min(t2.x, t2.y), t2.z);
    
    return t_near < t_far;
}

#define NUM_CUBES 1

const Cube cubes[NUM_CUBES] = { 
    Cube(vec3(1,1,1), vec3(2, 2, 2)),
    //Cube(vec3(8,5,7), vec3(1, 14, 1)),
    //Cube(vec3(4,3,4), vec3(2, 5, 3)),
};


void main() 
{
	// Set up initial data
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 image_size = imageSize(out_image);

	// Start in ndc sapce
	vec2 ndc = ((pixel_coords / image_size)) * 2.0 - 1.0;

	// Convert to eye space
	vec4 eye = vec4(vec2(inv_projection * vec4(ndc, -1.0, 1.0)), -1.0, 0.0);

	// Convert to world space
	vec4 world = inv_view * eye;

	vec3 ray_direction = normalize(world).xyz;

	Ray ray = Ray(position, ray_direction);

    bool hit_cube = false;
    for (int i = 0; i < NUM_CUBES; i++)
    {
        if (interesect_cube(ray, cubes[i]))
        {
            hit_cube = true;
        } 
    }

    if (hit_cube)
    {
        imageStore(out_image, pixel_coords, vec4((ray.direction.r + 1) / 2 - 0.1, (ray.direction.g + 1) / 2 - 0.1, (ray.direction.b + 1) / 2 - 0.1, 1));
    } 
    else 
    {
        imageStore(out_image, pixel_coords, vec4((ray.direction.r + 1) / 2, (ray.direction.g + 1) / 2, (ray.direction.b + 1) / 2, 1));
    }
}