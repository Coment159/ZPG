#ifndef MODEL_H
#define MODEL_H
#include "VAO.h"

class Model
{
public:
	Model() :vao(), vbo() {};
	Model(const float verticies[], GLuint size);
	GLint getSize() { return vbo.size; }
	void activate();
	void deactivate();

private:
	VAO vao;
	VBO vbo;
};



#endif // !1
