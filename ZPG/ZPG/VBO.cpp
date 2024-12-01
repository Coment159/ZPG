#include "VBO.h"

VBO::VBO(const float verticies[], GLuint size, int lenght) {
	glGenBuffers(1, &ID); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * lenght, verticies, GL_STATIC_DRAW);
	this->size = size;
}

VBO::VBO(Vertex* vertices, GLuint size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, vertices, GL_STATIC_DRAW);
	this->size = size;
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}