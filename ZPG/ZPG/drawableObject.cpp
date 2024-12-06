#include "drawableObject.h"
void DrawableObject::drawObject() {
	if (isActive)
	{
		shaderProgram->activate();
		model->activate();
		shaderProgram->setModelMatrix(transformations->getMatrix());
		if (material)
		{
			shaderProgram->setMaterial(material);
		}
		glStencilFunc(GL_ALWAYS, id, 0xFF);

		if (model->ibo != -1)
		{
			glDrawElements(GL_TRIANGLES, model->indiciesCount, GL_UNSIGNED_INT, NULL);
			shaderProgram->deactivate();
			model->deactivate();
			return;
		}

		glDrawArrays(GL_TRIANGLES, 0, model->getSize());//mode,first,count
		shaderProgram->deactivate();
		model->deactivate();
	}
}

