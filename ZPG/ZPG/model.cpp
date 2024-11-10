#include "model.h"

Model::Model(const float verticies[], GLuint size) : vao(), vbo(verticies, size) {
	vao.Bind();
	vao.LinkVBO(vbo, 0);
	vao.Unbind();
}

void Model::activate()
{
	vao.Bind();
}

void Model::deactivate() {
	vao.Unbind();
}

