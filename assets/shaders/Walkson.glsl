#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform writeonly image2D out_image;

uniform mat4 inv_projection;
uniform mat4 inv_view;

uniform vec3 position;

struct Ray {
	vec3 origin;
	vec3 direction;
};





void main() {
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


	imageStore(out_image, pixel_coords, vec4((ray.direction.r + 1) / 2, (ray.direction.g + 1) / 2, (ray.direction.b + 1) / 2, 1));
}