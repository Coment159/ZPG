#include "camera.h"

Camera::Camera()
	: position(glm::vec3(0.0f, 0.0f, 3.0f)),
	worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw(-90.0f), pitch(0.0f),
	front(glm::vec3(0.f, 0.f, -1.f))
{
	updateCameraVectors();
	//viewMatrix = glm::lookAt(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	viewMatrix = glm::lookAt(position, position + front, up);
	projectMatrix = glm::perspective(glm::radians(70.f), 800.f / 600.f, 0.1f, 100.0f);
}


void Camera::rotate(float yawOffset, float pitchOffset)
{
	yaw += yawOffset;
	pitch += pitchOffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	updateCameraVectors();  
	viewMatrix = glm::lookAt(position, position + front, up);  
	notifyAll(); 
}

void Camera::translate(glm::vec3 direction)
{
	float velocity = 0.05f;  
    position += direction * velocity;
    viewMatrix = glm::lookAt(position, position + front, up);
	notifyAll();
}


void Camera::updateCameraVectors()
{    
	glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
