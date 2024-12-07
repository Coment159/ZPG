#include "application.h"

/*
ZPG Project by TRU0101
*/

int main(void)
{

	Application* app = new Application();

	app->initialize();

	app->createShaders();
	app->createModels();

	app->run();


}

