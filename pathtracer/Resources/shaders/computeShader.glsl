#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba8, binding = 0) uniform image2D imgOutput;

uniform vec3 cameraPosition;
uniform mat4 inverseProj;
uniform mat4 inverseView;
uniform ivec2 imageSize;

uniform int bounces;

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Sphere {
    vec3 position;
    float radius;

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

struct Material {
    vec3 albedo;
    float roughness;
    float metallic;
    vec3 emissionColor;
    float emissionPower;

    vec3 getEmission() { return emissionColor * emissionPower; }
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

    /*
    float hitDistance = 20000; // view distance
    Surface* closestSurface = nullptr;

    for (auto& surfacePtr : viewPortData->scene->surfaces) {

        Surface& surface = *surfacePtr;


        float hitdist = surface.intersection(ray);

        if (hitdist > 0.0 && hitdist < hitDistance) {
            hitDistance = hitdist;
            closestSurface = &surface;
        }
    }
    */
    Sphere sphere;
    sphere.position = vec3(0.0f, 0.0f, 0.0f);
    sphere.radius = 1.0f;

    float hitDistance = sphere.intersection(ray);

    /*
    if (closestSurface == nullptr) {
        return miss(ray);
    }
    */
    if (hitDistance < 0.0f) {
        return miss(ray);
    }

    return closestHit(ray, hitDistance, sphere);
}

vec3 raygen(float x, float y) {

    // clean this stuff up soon
    // too much duplicate code in SSAA if statement

    vec3 light = vec3(0.0f, 0.0f, 0.0f); // variable to store the accumilated color from bounces
    vec3 backGroundColor = vec3(0.0f, 0.0f, 0.0f); // background color of scene
    vec3 energy = vec3(1.0f, 1.0f, 1.0f);

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
        //vec3 randVec = vec3(shiro_random_double(-1.0f, 1.0f), shiro_random_double(-1.0f, 1.0f), shiro_random_double(-1.0f, 1.0f));
        vec3 randVec = vec3(1.0f, 1.0f, 1.0f); // temp
        // while the vector is outside the unit sphere generate new til its not. this is so we can get a non bias vector
        /*
        while (glm::length2(randVec) > 1) {
            randVec = vec3(shiro_random_double(-1.0f, 1.0f), shiro_random_double(-1.0f, 1.0f), shiro_random_double(-1.0f, 1.0f));
        }
        */

        randVec = normalize(randVec); // create unit vector
        if (dot(randVec, payLoad.normal) < 0) { // if dot product is negative its pointing in wrong direction
            randVec = -randVec;
        }

        Material material; //*viewPortData->scene->materials[payLoad.surface->materialIndex];
        material.roughness = 1.0f;
        material.emissionPower = 1.0f;
        material.emissionColor = vec3(1.0f, 1.0f, 1.0f);
        material.albedo = vec3(0.0f,1.0f,0.0f);

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