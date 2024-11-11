#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h>

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "camera.h"
#include "Observer.h"
std::string get_file_contents(const char* filename);


class Camera;
class Shader : Observer {
public:

	Shader(const char* vertexFile, const char* fragmentFile);
	void addCamera(Camera* cam);
	void setUniformMatrix(glm::mat4& M);

	void update();

	void activate();
	void deactivate();
private:
	GLuint ID;
	GLuint modelMatrixId;
	GLuint viewMatrixId;
	GLuint projectMatrixId;
	Camera* cam;
};



#endif
