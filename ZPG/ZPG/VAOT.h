#pragma once
#include "VAO.h"
class VAOT : public VAO
{
public:
	VAOT() : VAO() {};

	void LinkVBO(VBO* VBO, GLuint layout) override;
	void LinkVBOVertex(VBO* VBO);
};

