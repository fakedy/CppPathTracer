#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba8, binding = 0) uniform image2D imgOutput;


uniform vec3 cameraPosition;
uniform mat4 inverseProj;
uniform mat4 inverseView;
uniform ivec2 imageSize;

uniform int bounces;

float randGen() {

    float val = fract(sin(dot(gl_GlobalInvocationID.xy/imageSize, vec2(12.9898, 78.233))) * 43758.5453);
    return (val*2) - 1.0f;
}

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Sphere {
    vec3 position;
    float radius;
    int materialIndex;

    float intersection(Ray ray)
    {
        vec3 rayDir = ray.direction;

        vec3 sphereToRayOrigin = ray.origin - position;
        // Equations to calculate hit on a sphere.
        float a = dot(rayDir, rayDir); 
        float b = 2.0f * dot(sphereToRayOrigin, rayDir);
        float c = dot(sphereToRayOrigin, sphereToRayOrigin) - (radius * radius);
        float disc = b * b - 4.0f * a * c;

        if (disc < 0) {
            return -1;
        }

        float t = ((-b - sqrt(disc)) / (2.0f * a));
        return t;
    }
};

layout(std430, binding = 1) buffer Spheres {
    Sphere spheres[];
};

struct Material {
    vec3 albedo;
    vec3 emissionColor;
    float roughness;
    float metallic;
    float emissionPower;

    vec3 getEmission() { return emissionColor * emissionPower; }
};

layout(std430, binding = 2) buffer Materials {
    Material materials[];
};
struct PayLoad {
    vec3 normal;
    float hitDistance;
    vec3 hitPosition;
    Sphere surface;
};

PayLoad miss(Ray ray)
{
    PayLoad payLoad;
    payLoad.hitDistance = -1;
    return payLoad;
}

PayLoad closestHit(Ray ray, float hitDistance, Sphere closestSurface)
{
    vec3 cameraPos = ray.origin - closestSurface.position;

    PayLoad payLoad;
    payLoad.hitDistance = hitDistance;
    payLoad.hitPosition = cameraPos + ray.direction * hitDistance;
    payLoad.normal = normalize(payLoad.hitPosition);
    payLoad.surface = closestSurface;
    payLoad.hitPosition += closestSurface.position;
    return payLoad;
}

PayLoad traceRay(Ray ray) {
    // fix this for glsl

    bool hitSphere = false;
    float hitDistance = 20000; // view distance
    Sphere closestSurface;

    for (int i = 0; i < spheres.length(); i++) {

        Sphere sphere = spheres[i];


        float hitdist = sphere.intersection(ray);

        if (hitdist > 0.0 && hitdist < hitDistance) {
            hitDistance = hitdist;
            closestSurface = sphere;
            hitSphere = true;
        }
    }


    if (hitSphere == false) {
        return miss(ray);
    }

    return closestHit(ray, hitDistance, closestSurface);
}

vec3 raygen(float x, float y) {


    vec3 light = vec3(0.0f); // variable to store the accumilated color from bounces
    vec3 backGroundColor = vec3(0.0f, 0.0f, 0.0f); // background color of scene
    vec3 energy = vec3(1.0f);

    vec2 coord = vec2((float(x)) / imageSize.x, float((y)) / imageSize.y); // translate pixel coordinate to clip space coord [0, 1]
    coord = coord * 2.0f - 1.0f; // remap the coordinates to [-1, 1]
    vec4 target = inverseProj * vec4(coord.x, coord.y, 1, 1);
    vec3 rayDir = vec3(inverseView * vec4(normalize(vec3(target) / target.w), 0));

    Ray ray;
    ray.origin = cameraPosition;
    ray.direction = normalize(rayDir);

    for (int i = 0; i < bounces; i++) {
        PayLoad payLoad = traceRay(ray);

        if (payLoad.hitDistance < 0) { // If we dont hit anything
            light += backGroundColor * energy;
            break;
        }

        ray.origin = payLoad.hitPosition + payLoad.normal * 0.0001f; // where we hit the surface + offset by normal dir to prevent hitting ourselves
        float randValue = randGen();
        vec3 randVec = vec3(randValue);

        // while the vector is outside the unit sphere generate new til its not. this is so we can get a non bias vector

        while (length(randVec) > 1) {
            float randValue = randGen();
            randVec = vec3(randValue);
        }

        randVec = normalize(randVec); // create unit vector
        if (dot(randVec, payLoad.normal) < 0) { // if dot product is negative its pointing in wrong direction
            randVec = -randVec;
        }

        Material material = materials[payLoad.surface.materialIndex];

        vec3 perturbedNormal = (payLoad.normal + material.roughness * randVec);
        ray.direction = normalize(reflect(ray.direction, perturbedNormal));


        light += material.getEmission() * energy;
        energy *= material.albedo;
    }

    return light;
}


void main() {
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    vec4 value = vec4(raygen(texelCoord.x, texelCoord.y),1.0f);

	imageStore(imgOutput, texelCoord, value);

}