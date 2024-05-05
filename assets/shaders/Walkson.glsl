#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;

uniform float fov;

void main()
{
	// Set up initial data
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 image_size = imageSize(screen);

	// Transform to clipspace
	vec2 ndc = ((pixel_coords / image_size) * 2) - 1;
	
	imageStore(screen, pixel_coords, vec4((ndc + 1.0 / 2.0), 1, 1));
}
