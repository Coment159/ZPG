#pragma once
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <GL/glew.h>
#include <SOIL.h>

class Material
{
public:
	Material();
	Material(
		glm::vec3 diffColor,
		glm::vec3 specColor,
		glm::vec3 ambientColor,
		float shine) :
		diffColor(diffColor),
		specColor(specColor),
		ambientColor(ambientColor),
		shine(shine),
		basicTexture(-1){};
	void addTexture(GLenum gl_texture, const char* texture_path);
	//Baci Prop
	glm::vec3 diffColor;
	glm::vec3 specColor;
	glm::vec3 ambientColor;
	float shine;

	//Texture Support
	GLuint basicTexture;
};

