#pragma once
#include "Observer.h"
#include "Light.h"
#include "IHasPosition.h"
#include "IHasFront.h"
class LightAttached : public Light, public Observer
{
public:
	LightAttached(Subject*  sub, LightType type = REFLECTOR, float angle = 30, glm::vec3 color = glm::vec3(1,1,1)):
	Light(type)
	{
		this->cutOff = angle;
		this->lightColor = color;
		sub->attach(this);
	};

	glm::vec3 getPosition() const override {
		return lightPosition;
	}

	void notify(Subject* sub) override {
		if (IHasPosition* pos = dynamic_cast<IHasPosition*>(sub))
		{
			lightPosition = pos->position;
		}
		if (IHasFront* front = dynamic_cast<IHasFront*>(sub))
		{
			lightDirection = front->front;
		}

		notifyAll();
	}
};



