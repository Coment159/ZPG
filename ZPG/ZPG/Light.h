#pragma once
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <memory>
#include "Subject.h"
#include "transformation.h"
class Light : public Subject
{
public:
	Light()
		: ambient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)),
		lightPosition(glm::vec3(0, 0, 0)),
		lightColor(glm::vec3(1, 1, 1)),
		radius(2.0f),
		angle(0.0f),
		speed(0.01f) {
		dynamicTranslation = std::make_unique<DynamicTranslation>(lightPosition, radius, speed);
	}
	Light(float radius, float angle, float speed)
		: ambient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)),
		lightPosition(glm::vec3(0, 0, 0)),
		lightColor(glm::vec3(1, 1, 1)),
		radius(radius),
		angle(angle),
		speed(speed) {
		dynamicTranslation = std::make_unique<DynamicTranslation>(lightPosition, radius, speed);
	}
	void circle() {
		dynamicTranslation->update();
		notifyAll();
	}




	glm::vec3 getPosition() const {
		return glm::vec3(dynamicTranslation->getMatrix() * glm::vec4(lightPosition,1));
	}


	glm::vec4 ambient;
	glm::vec3 lightPosition;
	glm::vec3 lightColor;
	float radius;
	float angle;
	float speed;

private:
	std::unique_ptr<DynamicTranslation> dynamicTranslation;
};

