#pragma once
#define GLM_FORCE_SSE2
#include <glm/glm.hpp>

class Material {

public:
	


	glm::vec3 albedo{ 1.0f };
	float roughness = 1.0f;
	float metallic = 0.0f;
	glm::vec3 emissionColor{ 0.0f };
	float emissionPower = 0.0f;

	glm::vec3 getEmission() const { return emissionColor * emissionPower;  }


};