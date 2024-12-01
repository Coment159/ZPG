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
	Model(std::string path, bool index = true);
	GLint getSize() { return vbo->size; }
	void activate();
	void deactivate();

	GLuint ibo = -1;
	GLuint indiciesCount = 0;

private:
	VAO* vao;
	VBO* vbo;
	
};



#endif // !1
