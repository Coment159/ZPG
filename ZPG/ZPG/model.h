#ifndef MODEL_H
#define MODEL_H
#include "VAOT.h"

#include<assimp/Importer.hpp>// C++ importerinterface
#include<assimp/scene.h>// aiSceneoutputdata structure
#include<assimp/postprocess.h>// Post processingflags


class Model
{
public:
	Model() :vao(), vbo() {};
	Model(const float verticies[], GLuint size, bool hasTexture = false);
	Model(std::string path);
	GLint getSize() { return vbo->size; }
	void activate();
	void deactivate();

private:
	VAO* vao;
	VBO* vbo;
};



#endif // !1
