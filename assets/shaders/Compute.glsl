#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;

uniform float fov;

void main()
{
	// Set up initial data
	vec4 pixel = vec4(0,0,1,0);
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 image_size = imageSize(screen);

	// Ray origin is the pixel coord transformed to [-1.0, 1.0]
	vec2 ndc = {
		-(float(pixel_coords.x * 2 - image_size.x) / image_size.x),
		-(float(pixel_coords.y * 2 - image_size.y) / image_size.y),
	};
	

	vec3 ray_origin = {
		ndc.x, ndc.y, 0.0
	};

	// Camera is at 0
	vec3 camera_origin = vec3(0.0, 0.0, -tan(fov / 2.0));
	vec3 ray_direction = normalize(ray_origin - camera_origin);

	// Definea sphere ig
	vec3 sphere_c = vec3(0.0, 0.0, -5.0);
	float sphere_r = 1.0;

	vec3 o_c = ray_origin - sphere_c;
	float b = dot(ray_direction, o_c);
	float c = dot(o_c, o_c) - sphere_r * sphere_r;
	float intersectionState = b * b - c;


	vec3 intersection = ray_origin + ray_direction * (-b + sqrt(b * b - c));

	if (intersectionState >= 0.0)
	{
		pixel = vec4((normalize(intersection - sphere_c) + 1.0) / 2.0, 1.0);
	}

	imageStore(screen, pixel_coords, pixel);
}
