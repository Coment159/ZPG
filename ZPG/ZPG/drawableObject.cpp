#include "drawableObject.h"
void DrawableObject::drawObject() {
	shaderProgram->activate();
	model->activate();
	shaderProgram->setModelMatrix(getModelMatrix());


	glDrawArrays(GL_TRIANGLES, 0, model->getSize());		 //mode,first,count

}

