#pragma once
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <memory>
#include "Subject.h"
#include "transformation.h"
enum LightType {
	SPOT = 1, DIRECT = 2 , REFLECTOR = 3
};
class Light : public Subject
{
public:
	/*
	Light()
		:lightPosition(glm::vec3(0, 0, 0)),
		lightColor(glm::vec3(1, 1, 1)),
		type(SPOT),
		radius(2.0f),
		angle(0.0f),
		speed(0.01f) {
		dynamicTranslation = std::make_unique<DynamicTranslation>(lightPosition, radius, speed);
	}
	*/
	
	Light(
		LightType type = SPOT, 
		glm::vec3 position = glm::vec3(0,0,0), 
		glm::vec3 lightDirection = glm::vec3(0,0,0),
		glm::vec3 color = glm::vec3(1,1,1),
		float radius = 0,
		float angle = 0,
		float speed = 0
		) :
		type(type), lightPosition(position), lightColor(color), lightDirection(lightDirection),
		radius(radius), angle(angle), speed(speed)
		{
		dynamicTranslation = std::make_unique<DynamicTranslation>(lightPosition, radius, speed);
		}

	Light(float radius, float angle, float speed, LightType type = SPOT)
		:lightPosition(glm::vec3(0, 0, 0)),
		lightColor(glm::vec3(1, 1, 1)),
		type(type),
		radius(radius),
		angle(angle),
		speed(speed) {
		dynamicTranslation = std::make_unique<DynamicTranslation>(lightPosition, radius, speed);
	}

	Light(glm::vec3 lighDir, glm::vec3 color, LightType type = DIRECT) :
		type(type), lightPosition(glm::vec3(0)), lightDirection(lighDir), lightColor(color),
		radius(0), angle(0), speed(0)
	{
		dynamicTranslation = std::make_unique<DynamicTranslation>(lightPosition, radius, speed);
	}

	void update() {
		dynamicTranslation->update();
		notifyAll();
	}




	virtual glm::vec3 getPosition() const {
		return glm::vec3(dynamicTranslation->getMatrix() * glm::vec4(lightPosition,1));
	}

	glm::vec3 getType() const {
		switch (type)
		{
		case SPOT:
			return glm::vec3(1, 0, 0);
			break;
		case DIRECT:
			return glm::vec3(0, 1, 0);
			break;
		case REFLECTOR:
			return glm::vec3(0, 0, 1);
			break;
		default:
			break;
		}
	}


	//glm::vec4 ambient;
	glm::vec3 lightPosition;
	glm::vec3 lightColor;
	glm::vec3 lightDirection;
	LightType type;

	std::unique_ptr<DynamicTranslation> dynamicTranslation;
	
	float radius;
	float angle;
	float speed;

	float cutOff;

private:

};

