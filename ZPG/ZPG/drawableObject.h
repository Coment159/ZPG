#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "model.h"
#include "shaderProgram.h"
#include "transformation.h"
#include "Material.h"

class DrawableObject {

public:
	DrawableObject(Model* model, ShaderProgram* shader, GLuint id)
		: model(model), shaderProgram(shader), id(id) {}

	DrawableObject(const DrawableObject& other, GLuint id)
		: model(other.model), shaderProgram(other.shaderProgram), id(id) {
		for (const auto& transform : other.transformations->transform) {
			transformations->addTransform(transform->clone());
		}
	}


	void setShaderProgram(ShaderProgram* shader) { shaderProgram = shader; }
	
	void drawObject();

	void addMaterial(Material* material) { this->material = material; }

	bool isActive = true;
	

	Transform* transformations = new Transform();

	GLuint getID() {return id;}

private:
	Model* model;
	ShaderProgram* shaderProgram;

	Material* material;

	GLuint id;


};
#endif // !DRAWABLEOBJECT_H
