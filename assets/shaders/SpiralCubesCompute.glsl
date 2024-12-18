#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform writeonly image2D out_image;

uniform float time;

vec3 palette (float t) 
{
    vec3 a = vec3(0.5, 0.5, 0.5);    
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

mat2 rotate_2d(float angle)
{
    return 
        mat2(
            cos(angle), -sin(angle),
            sin(angle), cos(angle)
        );
}


// https://jbaker.graphics/writings/DEC.html
float crazy_fracal( vec3 p0 ) 
{
    vec4 p = vec4(p0, 1.0);
    for(int i = 0; i < 8; i++)
    {
        p.xyz = mod(p.xyz - 1.0, 2.0) - 1.0;
        p *= 1.4 / dot(p.xyz, p.xyz);
    }
    return (length(p.xz / p.w ) * 0.25);
}

float sd_torus(vec3 p, float r)
{
    return length(vec2(p.y, length(p.xz) - r));
}

float sd_spehere(vec3 p, float size)
{
    return length(p) - size;
 
}

float sd_box(vec3 p, vec3 size)
{
    vec3 d = abs(p) - size;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float map(vec3 p) 
{
    // Continuously move the camera forwards
    p.z += time * 0.5;

    // Move the camera in a circle
    p.y += sin(time);
    p.x += cos(time);

        // Rotate  he cubes
    p.xy *= rotate_2d(sin(time) / 2 );
    p.xy *= rotate_2d(cos(time) / 2 );

    // Create an infinite field of cubes
    //p.xy = fract(p.xy) - 0.5;
    //p.z = mod(p.z, 0.5) - 0.25;


    return crazy_fracal(p);


//    return sd_box(p, vec3(0.1));
}



void main() 
{
	// Set up initial data
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 image_size = imageSize(out_image);

	// Start in ndc sapce
	vec2 uv = (pixel_coords * 2.0 - image_size.xy) / image_size.y;

    // Ray origin point 
    vec3 ro = vec3(0, 0, -3);
    
    // Ray direction
    float fov = 1.0;;
    vec3 rd = normalize(vec3(uv * fov, 1));

    vec3 colour = vec3(uv, 0);
    
    // Begin the march
    float t = 0;
    int i = 0;
    for (int i = 0; i < 80; i++) 
    {
        vec3 ray_position = ro + rd * t;

        // Spiral the ray as it gets further from the camera
        ray_position.xy *= rotate_2d(t * 0.9) * atan(time, sin(time));

        float dist = map(ray_position);
        t += dist;

        // Exit early if the ray gets too close to the object, or if the travel distance is over 100
        if (dist < 0.001 || t > 100.0) 
        {
            break;
        }
    }


    colour = palette(t * 0.05 + float(i) / 64.0);

    imageStore(out_image, pixel_coords, vec4(colour, 1));

}