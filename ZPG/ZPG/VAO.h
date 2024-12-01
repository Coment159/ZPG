#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include "VBO.h"

class VAO
{
public:
	VAO();

	virtual void LinkVBO(VBO* VBO, GLuint layout);

	void Bind();
	void Unbind();
	void Delete();

	GLuint ID = 0;
};


#endif // !VAO_CLASS_H
