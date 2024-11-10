#ifndef SCENE_H
#define SCENE_H
#include <vector>

#include "shaderProgram.h"
#include "drawableObject.h"
#include "Light.h"

class Scene
{
public:
	Scene(const char* vertFile, const char* fragFile);
	void renderScene();



	ShaderProgram* createShader(const char* vertFile, const char* fragFile);


	DrawableObject* addObject(const float points[], GLuint size,ShaderProgram* shader);
	DrawableObject* addObject(const float points[], GLuint size);

	DrawableObject* getObject(int index);
	DrawableObject* duplicateObject(int index);
	DrawableObject* duplicateObject(DrawableObject* obj);

	void clearObjects();

	void addSubjectToShader(Subject* subject);
	void addSubjectToShader(Subject* subject, ShaderProgram* shader);





	std::vector<DrawableObject*> objects;
	std::vector<ShaderProgram*> shaders;
	std::vector<Light*> lights;

	Camera* cam;
	Light* light = new Light();


private:

};



#endif // !SCENE_H

