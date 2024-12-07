#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <unordered_map>

#include "shaderProgram.h"
#include "drawableObject.h"
#include "LightAttached.h"
#include "Sykbox.h"

class Scene
{
public:
	Scene(const char* vertFile, const char* fragFile);
	void renderScene();

	ShaderProgram* createShader(const char* vertFile, const char* fragFile, const char* name);


	DrawableObject* addObject(Model* model,ShaderProgram* shader);
	DrawableObject* addObject(Model* model);

	DrawableObject* getObject(int index);
	DrawableObject* duplicateObject(int index);
	DrawableObject* duplicateObject(DrawableObject* obj);

	void clearObjects();

	void addSubjectToShader(Subject* subject);
	void addSubjectToShader(Subject* subject, ShaderProgram* shader);

	std::vector<DrawableObject*> objects;
	// unordered map
	std::unordered_map<std::string, ShaderProgram*> shaders;
	std::vector<Light*> lights;

	Camera* cam;
	Light* light = new Light();
	
	Skybox* skybox;

private:
	GLuint newId = 0;
};



#endif // !SCENE_H

