#include "transformation.h"

glm::mat4 TransformCompozite::getMatrix()
{
	for (auto& trans : transform) {
		if (auto dynamic = dynamic_cast<DynamicTransformation*>(trans.get()))
		{
			dynamic->update();
		}
	}
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	for (const auto& transform : transform) {
		modelMatrix *= transform->getMatrix();
	}
	return modelMatrix;
}


void TransformCompozite::addTransform(std::unique_ptr<BasicTransform> trans)
{
	transform.push_back(std::move(trans));
}
