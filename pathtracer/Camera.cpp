#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, float fov, uint32_t width, uint32_t height)
{
	this->cameraPos = position;
	this->fov = fov;
	this->width = width;
	this->height = height;
	calculateView();
	calculateProj();
	calcDirections();
}

void Camera::resize(uint32_t width, uint32_t height)
{
	this->width = width;
	this->height = height;
	calculateView();
	calculateProj();
	calcDirections();
}

void Camera::calculateView()
{
	view = glm::lookAt(cameraPos, cameraPos + lookDir, upDir);
	inverseView = glm::inverse(view);
}

void Camera::calculateProj()
{
	proj = glm::perspectiveFov(glm::radians(fov), (float)width, (float)height, nearClip, farClip);
	inverseProj = glm::inverse(proj);
}

void Camera::calcDirections()
{
	directions.resize(width * height);
	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			glm::vec2 coord = glm::vec2((float)x / width, (float)y / height); // translate pixel coordinate to clip space coord [0, 1]
			coord = coord * 2.0f - 1.0f; // remap the coordinates to [-1, 1]
			glm::vec4 target = inverseProj * glm::vec4(coord.x, coord.y, 1, 1);
			glm::vec3 rayDir = glm::vec3(inverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // world space
			directions[x + y * width] = rayDir;
		}
	}
}

