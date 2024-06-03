#pragma once
#include <glm/glm.hpp>
#include <vector>

class Camera
{


public:
	Camera(glm::vec3 position, float fov, uint32_t width, uint32_t height);

	glm::vec3 cameraPos;
	const glm::vec3& getPosition() const { return cameraPos; }
	const std::vector<glm::vec3>& getDirections() const { return directions; }

	void resize(uint32_t width, uint32_t height);
	void update();


private:

	const glm::mat4& getProj() const { return proj; }
	const glm::mat4& getInversProj() const { return inverseProj; }
	const glm::mat4& getViewMatrix() const { return view; }
	const glm::mat4& getInverseView() const { return inverseView; }


	glm::mat4 proj{1.0f};
	glm::mat4 inverseProj{1.0f};
	glm::mat4 view{1.0f};
	glm::mat4 inverseView{1.0f};
	float fov;
	float nearClip{0.01f};
	float farClip{100.0f};

	uint32_t width;
	uint32_t height;

	
	std::vector<glm::vec3> directions; // one for each pixel on screen
	glm::vec3 lookDir{0,0,-1};
	glm::vec3 upDir{0,1,0};



	// goal is we calculate direction of each ray as it travels from camera in direction of perspective

	void calculateView();
	void calculateProj();
	void calcDirections();

};

