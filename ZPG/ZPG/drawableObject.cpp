#include "drawableObject.h"
void DrawableObject::drawObject() {
	shaderProgram->activate();
	model->activate();
	shaderProgram->setModelMatrix(getModelMatrix());
	if (material)
	{
		shaderProgram->setMaterial(material);
	}

	if (model->ibo != -1)
	{
		glDrawElements(GL_TRIANGLES, model->indiciesCount, GL_UNSIGNED_INT, NULL);
		return;
	}

	glDrawArrays(GL_TRIANGLES, 0, model->getSize());		 //mode,first,count

}

