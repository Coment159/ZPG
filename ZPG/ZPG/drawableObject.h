#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "model.h"
#include "shaderProgram.h"
#include "transformation.h"
#include "Material.h"
class DrawableObject {

public:
	//DrawableObject(Model* model, Shader* shader) : model(model), shader(shader
	DrawableObject(Model* model, ShaderProgram* shader)
		: model(model), shaderProgram(shader) {}

	DrawableObject(const DrawableObject& other)
		: model(other.model), shaderProgram(other.shaderProgram) {
		for (const auto& transform : other.transformations) {
			transformations.push_back(transform->clone());
		}
	}


	void setShaderProgram(ShaderProgram* shader) { shaderProgram = shader; }
	
	void drawObject();

	void addMaterial(Material* material) { this->material = material; }


	void addTranslation(float x, float y, float z) {
		transformations.push_back(std::make_unique<Translation>(x, y, z));
	}

	void addRotation(float angle, float x, float y, float z) {
		transformations.push_back(std::make_unique<Rotation>(angle, x, y, z));
	}

	void addScaling(float scale) {
		transformations.push_back(std::make_unique<Scaling>(scale));
	}

	void addDynamicRotation(float speed, float x, float y, float z) {
		transformations.push_back(std::make_unique<DynamicRotation>(glm::vec3(x, y, z), speed));
	}

	void resetTransformations() {
		transformations.clear();
	}

	void updateDynamicTransformations() {
		for (auto& transform : transformations) {
			if (auto dynamicRotation = dynamic_cast<DynamicRotation*>(transform.get())) {
				dynamicRotation->update();
			}
		}
	}


private:
	//Transformation data
	Model* model;
	ShaderProgram* shaderProgram;
	std::vector<std::unique_ptr<BasicTransform>> transformations;
	Material* material;


	glm::mat4 getModelMatrix() const {
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		for (const auto& transform : transformations) {
			modelMatrix *= transform->getMatrix();
		}
		return modelMatrix;
	}


};
#endif // !DRAWABLEOBJECT_H
