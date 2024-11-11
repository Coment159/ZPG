#include "application.h"


Application::Application()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);


	// get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float ratio = width / (float)height;
	glViewport(0, -50, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
}

Application::~Application()
{
}

void Application::initialize()
{
	instance = this;

	//1st scene
	scene.push_back(new Scene("default.vert.txt", "default.frag.txt"));
	activeSceneInt = 0;
	activeScene = scene.at(activeSceneInt);
	activeScene->cam = camera;
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->cam));

	//2nd scene
	scene.push_back(new Scene("default.vert.txt", "phongForest.frag.txt"));
	Scene* scene2 = scene.at(1);
	scene2->cam = camera;
	scene2->addSubjectToShader(dynamic_cast<Subject*>(scene2->cam));
	scene2->light->lightPosition = glm::vec3(0, 1, 0);
	scene2->lights.push_back(new Light(2.0f,0,0.01f));
	scene2->lights.at(1)->lightPosition = glm::vec3(0,1,2);
	scene2->lights.at(1)->speed *= 2;

	//3rd scene
	scene.push_back(new Scene("default.vert.txt", "phong.frag.txt"));
	Scene* scene3 = scene.at(2);
	scene3->cam = camera;
	scene3->addSubjectToShader(dynamic_cast<Subject*>(scene3->cam));
	scene3->light = new Light();


	//4rd scene
	/*
	scene.push_back(new Scene("default.vert.txt", "default.frag.txt"));
	Scene* scene4 = scene.at(3);
	scene4->addSubjectToShader(dynamic_cast<Subject*>(scene4->cam));
	*/
}

void Application::createShaders()
{
	Scene* sc = scene[3];
	sc->createShader("default.vert.txt", "lambert.frag.txt");
	sc->createShader("default.vert.txt", "phong.frag.txt");
	sc->createShader("default.vert.txt", "blinn.frag.txt");

}

void Application::createModels()
{
	activeScene = scene[1];
	createForest();
	activeScene = scene[2];
	create3rdScene();
	//activeScene = scene[3];
	//create4thScene();
	
	activeScene = scene[0];
	auto obj =  activeScene->addObject(triangle, triangleSize);
	

}

void Application::run()
{
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (!glfwWindowShouldClose(window)) {
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);
		// clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		activeScene->renderScene();

		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window);

		// update other events like input handling
		glfwPollEvents();

		// Measure speed  !!!!!!!
		double currentTime = glfwGetTime();
		nbFrames++;
		// 60fps = 16.6666ms
		// 30fps = 33.3333ms.
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer]
			int FPS = 1000 / (1000.0 / double(nbFrames));
			printf("%d FPS\n", FPS);
			nbFrames = 0;
			lastTime += 1.0;
		}
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Application::downScale()
{
	for (auto object : activeScene->objects) {
		object->addScaling(0.75f);
		//object->scale(0.75f);
	}
}
void Application::upScale()
{
	for (auto object : activeScene->objects) {
		object->addScaling(1.25f);
		//object->scale(1.25f);
	}
}
void Application::treeRight()
{
	for (auto object : activeScene->objects) {
		object->addTranslation(0.1f,0,0);
		//object->translate(0.1f,0,0);
	}
}
void Application::treeLeft()
{
	for (auto object : activeScene->objects) {
		object->addTranslation(-0.1f, 0, 0);
		//object->translate(-0.1f, 0, 0);
	}
}
void Application::treeUp()
{
	for (auto object : activeScene->objects) {
		object->addTranslation(0, 0.25f, 0);
		//object->translate(0, 0.25f, 0);
		//object->translate(0, 0, 0.1f);
	}
}
void Application::treeDown()
{
	for (auto object : activeScene->objects) {
		object->addTranslation(0, -0.25f, 0);
		//object->translate(0, -0.25f, 0);
		//object->translate(0, 0, -0.1f);
	}
}

void Application::changeScene()
{
	activeSceneInt++;
	if (activeSceneInt >= scene.size())
	{
		activeSceneInt = 0;
	}
	activeScene = scene.at(activeSceneInt);
	//activeScene->clearObjects();
	//createModels();
}


void Application::createForest()
{
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->light));
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->lights.at(1)));

	
	auto obj = activeScene->addObject(tree, treeSize);
	
	for (size_t i = 0; i < 50; i++)
	{
		float randomX = (static_cast<float>(rand()) / RAND_MAX) * 40- 20.f;
		float randomAngle = (static_cast<float>(rand()) / RAND_MAX)* 360 - 180.f;
		auto randomObject = activeScene->duplicateObject(obj);
		randomObject->addScaling(0.1f);
		randomObject->addTranslation(randomX, 0, i);
		randomObject->addRotation(randomAngle, 0, 1 , 0);
	}
	obj->addScaling(0.1f);
	obj = activeScene->addObject(bushes, bushesSize);
	for (size_t i = 0; i < 200; i++)
	{
		float randomX = (static_cast<float>(rand()) / RAND_MAX) * 20 - 10.f;
		float randomAngle = (static_cast<float>(rand()) / RAND_MAX) - 0.5f;

		auto randomObject = activeScene->duplicateObject(obj);
		randomObject->addScaling(0.25f);
		randomObject->addTranslation(randomX, 0, i*0.1f);
		randomObject->addRotation(randomAngle, 0, 1, 0);
	}

	obj->addScaling(0.25f);

	int random = rand() % 100;
	for (auto& o : activeScene->objects) {
		if (random < 25)
		{
			o->addDynamicRotation(0.01f, 0, 1, 0);
		}
		random = rand() % 100;
	}

	auto floor = activeScene->addObject(plain, plainSize);
	floor->addScaling(5);
	floor->addTranslation(0, 0, 0.5f);
}

void Application::create3rdScene()
{
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->light));
	activeScene->light->lightPosition = glm::vec3(0,0,0);

	auto obj = activeScene->addObject(sphere, sphereSize);

	obj->addTranslation(2, 0, 0);

	obj = activeScene->duplicateObject(obj);
	obj->addTranslation(-2, 2, 0);
	

	obj = activeScene->duplicateObject(obj);
	obj->addTranslation(-2, -2, 0);

	obj = activeScene->duplicateObject(obj);
	obj->addTranslation(2, -2, 0);
}

void Application::create4thScene()
{
	Light* light = activeScene->light;
	light->lightPosition = glm::vec3(0, 0, 0);
	//light->lightColor = glm::vec3(1, 0, 0);

	ShaderProgram* defaultShader = activeScene->shaders[0];
	defaultShader->addSubject(dynamic_cast<Subject*>(light));
	auto obj = activeScene->addObject(sphere, sphereSize, defaultShader);
	obj->addTranslation(2, 0, 0);
	
	ShaderProgram* lambertShader = activeScene->shaders[1];
	lambertShader->addSubject(dynamic_cast<Subject*>(light));
	obj = activeScene->duplicateObject(obj);
	obj->setShaderProgram(lambertShader);
	obj->addTranslation(-2, 2, 0);
	
	ShaderProgram* phongShader = activeScene->shaders[2];
	phongShader->addSubject(dynamic_cast<Subject*>(light));
	obj = activeScene->duplicateObject(obj);
	obj->setShaderProgram(phongShader);
	obj->addTranslation(-2, -2, 0);

	ShaderProgram* blinnShader = activeScene->shaders[3];
	blinnShader->addSubject(dynamic_cast<Subject*>(light));
	obj = activeScene->duplicateObject(obj);
	obj->setShaderProgram(blinnShader);
	obj->addTranslation(2, -2, 0);
	
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (app->activeScene->cam)
	{
		glm::vec3 direction(0.0f);

		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
			direction += app->activeScene->cam->front; 
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
			direction -= app->activeScene->cam->front; 
		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
			direction -= app->activeScene->cam->right; 
		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
			direction += app->activeScene->cam->right; 

		app->activeScene->cam->translate(direction);
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		app->changeScene();
	}
	
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

void Application::cursor_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; 
    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.1f;  
    xOffset *= sensitivity;
    yOffset *= sensitivity;

	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app->activeScene->cam)
    {
		app->activeScene->cam->rotate(xOffset, yOffset);
    }
}

void Application::window_size_callback(GLFWwindow* window, int width, int height)
{
	float ration = width / (float)height;
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	app->camera->projectMatrix = glm::perspective(glm::radians(70.f), ration, 0.1f, 100.f);
	glViewport(0, 0, width, height);
	app->camera->notifyAll();
}



