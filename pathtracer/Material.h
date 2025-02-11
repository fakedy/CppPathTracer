#pragma once
#define GLM_FORCE_SSE2
#include <glm/glm.hpp>

class alignas(16) Material {

public:
	
	alignas(16) glm::vec3 albedo{ 1.0f };
	alignas(16) glm::vec3 emissionColor{ 0.0f };
	float roughness{ 1.0f };
	float metallic{ 0.0f }; 
	float emissionPower{ 0.0f };
	float pad{};


	glm::vec3 getEmission() const { return emissionColor * emissionPower;  }


};