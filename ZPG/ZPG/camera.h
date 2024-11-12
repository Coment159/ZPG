#ifndef CAMERA_H
#define CAMERA_H
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <vector>

#include "IHasPosition.h"
#include "IHasFront.h"
#include "Subject.h"
class Camera : public Subject, public IHasPosition, public IHasFront
{
public:
	Camera();

	void rotate(float yaw, float pitch); 
	void translate(glm::vec3 direction);
	glm::mat4 viewMatrix;
	glm::mat4 projectMatrix;	
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

private:

	void updateCameraVectors();
};




#endif // !CAMERA_H

