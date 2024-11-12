#include "scene.h"


Scene::Scene(const char* vertFile, const char* fragFile)
{
	//defaultShader = shaderProgram->createShader(vertFile, fragFile);
	shaders.push_back(new ShaderProgram(vertFile, fragFile));
	lights.push_back(light);
}

DrawableObject* Scene::addObject(const float points[],GLuint size , ShaderProgram* shader)
{

	objects.push_back(new DrawableObject(points, size , shader));
	return objects.at(objects.size() - 1);
}

DrawableObject* Scene::addObject(const float points[], GLuint size)
{
	objects.push_back(new DrawableObject(points, size ,shaders.at(0)));
	return objects.at(objects.size() - 1);;
}

void Scene::clearObjects()
{
	for (auto obj : objects) {
		delete(obj);
	}
	objects.clear();
}


void Scene::addSubjectToShader(Subject* subject)
{
	shaders.at(0)->addSubject(subject);
}

void Scene::addSubjectToShader(Subject* subject, ShaderProgram* shader)
{
	shader->addSubject(subject);
}

void Scene::renderScene()
{
	
	for (auto& light :lights) {
		if (LightAttached* l = dynamic_cast<LightAttached*>(light))
		{
			continue;
		}
		light->update();
	}
	
	for (auto i : objects) {
		i->updateDynamicTransformations();
		i->drawObject();
	}

}

DrawableObject* Scene::getObject(int index)
{
	return objects.at(index);
}

DrawableObject* Scene::duplicateObject(int index)
{
	objects.push_back(new DrawableObject(*objects.at(index)));

	return objects.at(objects.size()-1);
}

DrawableObject* Scene::duplicateObject(DrawableObject* obj)
{
	objects.push_back(new DrawableObject(*obj));
	return objects.at(objects.size() - 1);
}

ShaderProgram* Scene::createShader(const char* vertFile, const char* fragFile) {
	int size = shaders.size();
	shaders.push_back(new ShaderProgram(vertFile, fragFile));
	shaders.at(size)->addSubject(dynamic_cast<Subject*>(cam));
	return shaders.at(size);


}
