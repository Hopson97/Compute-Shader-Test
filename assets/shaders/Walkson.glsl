#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform writeonly image2D out_image;



void main() {
	// Set up initial data
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 image_size = imageSize(out_image);

	// Transform to clipspace
	vec2 ndc = ((pixel_coords / image_size)) * 2.0 - 1.0;
	


	// Transform to a direction from 
	imageStore(out_image, pixel_coords, vec4((ndc.r + 1) / 2, (ndc
	.g + 1) / 2, 0, 1));
}