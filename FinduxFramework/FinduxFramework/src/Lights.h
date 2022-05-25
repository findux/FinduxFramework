#pragma once

#include "glm/glm.hpp"

class Light
{
public:
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	int on = 1;
};

class DirectionalLight : public Light
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);;
};

class PointLight : public Light
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f);
	float constant = 1.9f;
	float linear = 0.22f;
	float exponent = 0.20f;
};

class SpotLight : public Light
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f);
	float constant = 1.9f;
	float linear = 0.22f;
	float exponent = 0.20f;
	float cosInnerCone = glm::cos(glm::radians(15.0f));
	float cosOuterCone = glm::cos(glm::radians(20.0f));
};

