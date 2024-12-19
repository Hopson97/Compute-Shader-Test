#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform writeonly image2D out_image;


// Kind of SDF to render based on selection from the GUI
#define KIND_TORUS 0
#define KIND_CUBES 1
#define KIND_FRACTAL1 2
#define KIND_FRACTAL2 3
uniform int kind;

// Time since the simulation started
uniform float time;

// How much distortion to apply to the shapes
uniform float distortion;

// How fast to move the camera forwards
uniform float movement_speed;

// Colour pallete from https://www.shadertoy.com/view/ll2GD3
vec3 palette (float t) 
{
    vec3 a = vec3(0.5, 0.5, 0.5);    
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

// Standard 2D rotation using matrices
mat2 rotate_2d(float angle)
{
    return 
        mat2(
            cos(angle), -sin(angle),
            sin(angle), cos(angle)
        );
}

// Same as "fractal", just without the 4d vector
float fire_spiral(vec3 p) 
{
    for(int i = 0; i < 8; i++)
    {
        p.xyz = mod(p.xyz - 1.0, 2.0) - 1.0;
        p *= 1.4 / dot(p.xyz, p.xyz);
    }
    return (length(p.xz) * 0.25);
}

// All fracal and SDF functions are from/adapted from https://jbaker.graphics/writings/DEC.html
float fracal(vec3 p0) 
{   
    vec4 p = vec4(p0, 1.0);
    for(int i = 0; i < 8; i++)
    {
        p.xyz = mod(p.xyz - 1.0, 2.0) - 1.0;
        p *= 1.4 / dot(p.xyz, p.xyz);
    }
    return (length(p.xz / p.w ) * 0.25);
}

float fracal2(vec3 p) 
{
    p.xz = fract(p.xz) - 0.5;
    float k = 1.0;
    float s = 0.0;

    for (int i = 0; i < 8 ; i++) 
    {
        s = 2.0 / clamp(dot(p, p /* * (sin(time) + 1.5) / 2*/), 0.1, 1.0);
        p = abs(p) * s - vec3(0.5, 3.0, 0.5);
        k *= s;
    }
    return length(p) / k - 0.001;
}

// By kamoshika
#define X(V) d = min(d, length(V) - 0.13)
float fracal3(vec3 p)
{
    vec3 R = p, Q;
    float d = 1.;
    Q=fract(R)-.5;
    X(Q.xy);
    X(Q.yz);
    X(Q.zx);
    d=max(d,.68-length(fract(R-.5)-.5));
    return d;
  }

float sd_torus(vec3 p, float r)
{
    return length(vec2(p.y, length(p.xz) - r));
}

float sd_sphere(vec3 p, float size)
{
    return length(p) - size;
 
}

float sd_box(vec3 p, vec3 size)
{
    vec3 d = abs(p) - size;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float map_fracatals(in vec3 p) 
{
    // Move the camera in a circle
    p.y += sin(time) - 1;
    p.x += cos(time);

    // Rotate  the fracal
    p.xy *= rotate_2d(sin(time) / 2);
    p.xy *= rotate_2d(cos(time) / 2);
    
    // return mix(fracal(p), fracal2(p), (sin(time / 10) + 1) / 2);
    if (kind == KIND_FRACTAL1) 
    {
        return fracal(p);
    }
    else if (kind == KIND_FRACTAL2) 
    {
        return fracal2(p);
    }
}

float map_primatives(in vec3 p) 
{
    // Rotate the shapes
    //p.xy *= rotate_2d((time) / 64 );

    // Create an infinite field
    p.xy = fract(p.xy) - 0.5;
    p.z = mod(p.z, 0.5) - 0.25;

    if (kind == KIND_CUBES) 
    {
        return sd_box(p, vec3(0.1));
    }
    else if (kind == KIND_TORUS) 
    {
        return sd_torus(p, 0.25);
    }
}


void main() 
{
	// Set up initial data
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 image_size = imageSize(out_image);

	// Start in ndc sapce
	vec2 uv = (pixel_coords * 2.0 - image_size.xy) / image_size.y;
     
    // Ray origin point 
    vec3 ro = vec3(0, 1, -3);
    
    // Ray direction
    float fov = 1.0;;
    vec3 rd = normalize(vec3(uv * fov, 1));
    
    // Begin the march
    float t = 0;
    int i = 0;
    for (i = 0; i < 80; i++) 
    {
        vec3 ray_position = ro + rd * t;

        // Spiral the ray as it gets further from the camera
        ray_position.xy *= rotate_2d(t * distortion);// * atan(time, sin(time));

        // Continuously move the camera forwards
        ray_position.z += time * movement_speed;

        // Get the nearest signed-distance field distance
        float dist = 0;
        switch(kind)
        {
            case KIND_TORUS:
            case KIND_CUBES:
                dist = map_primatives(ray_position);
                break;

            case KIND_FRACTAL1:
            case KIND_FRACTAL2:
                dist = map_fracatals(ray_position);
                break;

        }

        // Step the travel based on the last sdf
        t += dist;

        // Exit early if the ray gets too close to the object, or if the travel distance is over 100
        if (dist < 0.001 || t > 100.0) 
        {
            break;
        }
    }


    vec3 colour = palette(t * 0.05 + (float(i) / 80.0));

    imageStore(out_image, pixel_coords, vec4(colour, 1));

}