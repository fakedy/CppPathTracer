#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "Material.h"


class Scene {
public:

    std::vector<std::shared_ptr<Surface>> surfaces;
    std::vector<std::shared_ptr<Material>> materials;


    Scene() {

        /*
        glm::vec3 albedo{ 1.0f };
	    float roughness = 1.0f;
        float metallic = 0.0f;
        glm::vec3 emissionColor{ 0.0f };
        float emissionPower = 0.0f;
        */

        auto material1 = std::make_shared<Material>();
        auto material2 = std::make_shared<Material>();
        auto material3 = std::make_shared<Material>();
        auto material4 = std::make_shared<Material>();
        auto material5 = std::make_shared<Material>();

        material1->albedo = glm::vec3(1, 0, 1);
        material1->roughness = 0.2f;
        material1->metallic = 0.0f;

        material2->albedo = glm::vec3(0.08, 0.08, 0.08);
        material2->roughness = 0.2f;
        material2->metallic = 0.0f;

        material3->albedo = glm::vec3(0, 0, 0);
        material3->roughness = 0.02f;
        material3->metallic = 0.0f;

        material4->albedo = glm::vec3(0.8, 0.1, 0.1);
        material4->roughness = 0.2f;
        material4->metallic = 0.0f;

        material5->albedo = glm::vec3(0.1, 0.3, 0.5);
        material5->roughness = 0.2f;
        material5->metallic = 0.0f;

        auto sphere1 = std::make_shared<Sphere>();
        auto sphere2 = std::make_shared<Sphere>();
        auto sphere3 = std::make_shared<Sphere>();
        auto sphere4 = std::make_shared<Sphere>();
        auto sphere5 = std::make_shared<Sphere>();

        sphere1->name = "Purple Sphere";
        sphere1->position = glm::vec3(-2, 1, 0);
        sphere1->radius = 1.0f;
        sphere1->materialIndex = 0;

        sphere2->name = "Big Floor Sphere";
        sphere2->position = glm::vec3(0, -101.5, 0);
        sphere2->radius = 100.0f;
        sphere2->materialIndex = 1;

        sphere3->name = "Reflective Sphere";
        sphere3->position = glm::vec3(2, -1, 0);
        sphere3->radius = 1.5f;
        sphere3->materialIndex = 2;

        sphere4->name = "Red Sphere";
        sphere4->position = glm::vec3(-1, -1, -9);
        sphere4->radius = 3.0f;
        sphere4->materialIndex = 3;

        sphere5->name = "Blueish Sphere";
        sphere5->position = glm::vec3(-1, -1.1, 0.1);
        sphere5->radius = 0.5f;
        sphere5->materialIndex = 4;


        surfaces.push_back(sphere1);
        surfaces.push_back(sphere2);
        surfaces.push_back(sphere3);
        surfaces.push_back(sphere4);
        surfaces.push_back(sphere5);

        materials.push_back(material1);
        materials.push_back(material2);
        materials.push_back(material3);
        materials.push_back(material4);
        materials.push_back(material5);
    }
};

