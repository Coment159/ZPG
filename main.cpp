#include "application.h"


//application, shaderprogram id nesmi opustit shaderprogram, drawableobject(model) 

int main(void)
{

	Application* app = new Application();

	app->initialize();

	//app->createShaders();
	app->createModels();

	app->run();


}

