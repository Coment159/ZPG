#include "drawableObject.h"
void DrawableObject::drawObject() {
	shaderProgram->activate();
	model->activate();
	shaderProgram->setModelMatrix(getModelMatrix());
	if (material)
	{
		shaderProgram->setMaterial(material);
	}


	glDrawArrays(GL_TRIANGLES, 0, model->getSize());		 //mode,first,count

}

