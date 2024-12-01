#include "VAO.h"

VAO::VAO() {
	
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO* VBO, GLuint layout) {

	 //enable vertex attributes
	Bind();
	VBO->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	VBO->Unbind();
	Unbind();
}

void VAO::Bind() {
	glBindVertexArray(ID); //bind the VAO
}

void VAO::Unbind() {
	glBindVertexArray(0); //unbind the VAO
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}