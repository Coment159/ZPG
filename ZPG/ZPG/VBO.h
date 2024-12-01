#ifndef VBO_CLASS_H
#define VBO_CLASS_H

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h>


//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Vertex
{
	float Position[3];
	float Normal[3];
	float Texture[2];
	float Tangent[3];
};

class VBO
{
public:
	VBO() {};
	VBO(const float verticies[], GLuint size, int lenght = 6);
	VBO(Vertex* vertices, GLuint size);

	void Bind();
	void Unbind();
	void Delete();

	GLuint ID = 0;
	GLuint size = 0;
};


#endif // !VBO_CLASS_H

