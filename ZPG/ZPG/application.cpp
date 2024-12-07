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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

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
	glViewport(0, 0, width, height);

	int a;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &a);
	printf("Texture units: %d\n", a);
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
	activeScene->lights.push_back(new Light(0,0,0));



	//2nd scene
	scene.push_back(new Scene("default.vert.txt", "phongForest.frag.txt"));
	Scene* scene2 = scene.at(1);
	scene2->cam = camera;
	scene2->addSubjectToShader(dynamic_cast<Subject*>(scene2->cam));
	scene2->light->lightPosition = glm::vec3(0, -5, 0);


	scene2->lights.push_back(new Light(2.0f,0,0.01f));
	scene2->lights.at(1)->lightPosition = glm::vec3(0,1,2);
	scene2->lights.at(1)->speed *= 2;


	scene2->lights.push_back(new Light(2.0f, 0, -0.01f));
	scene2->lights.at(2)->lightPosition = glm::vec3(1, 1, 2);
	//scene2->lights.at(2)->speed *= 2;

	scene2->lights.push_back(new LightAttached(camera,REFLECTOR , 30.f));

	//3rd scene
	scene.push_back(new Scene("default.vert.txt", "phongForest.frag.txt"));
	Scene* scene3 = scene.at(2);
	scene3->cam = camera;
	scene3->addSubjectToShader(dynamic_cast<Subject*>(scene3->cam));
	scene3->light = new Light(0,0,0);
	scene3->lights.push_back(new Light(0,0,0));
}

void Application::createShaders()
{

	shaders["textureShader"] = new ShaderProgram("textureVertex.txt", "textureFragment.txt");
	shaders["default"] = new ShaderProgram("default.vert.txt", "default.frag.txt");
	shaders["nemec"] = new ShaderProgram("Shaders/PhongVertexShader.glsl", "Shaders/PhongFragmentShader.glsl");
}

void Application::createModels()
{
	models["tree"] = new Model(tree, treeSize);
	models["bushes"] = new Model(bushes, bushesSize);
	models["triangle"] = new Model(triangle, triangleSize);
	models["plain"] = new Model(plain, plainSize);
	models["sphere"] = new Model(sphere, sphereSize);
	models["triangleTextured"] = new Model(triangleT, 6, true);
	models["plainTextured"] = new Model(triangleT, plainSize, true);

	models["login"] = new Model("Models/login.obj");
	models["house"] = new Model("Models/house.obj");
	models["zoombi"] = new Model("Models/zombie.obj");
	//models["car"] = new Model("Models/car.obj",false);
	models["treeObj"] = new Model("Models/tree.obj");
	

	materials["triangleTextured2"] = new Material();
	materials["triangleTextured2"]->addTexture(GL_TEXTURE0, "Textures/test.png");
	materials["grassTexture"] = new Material();
	materials["grassTexture"]->addTexture(GL_TEXTURE1, "Textures/grass.png");
	materials["houseTextured"] = new Material();
	materials["houseTextured"]->addTexture(GL_TEXTURE2, "Textures/house.png");
	materials["zoombieTextured"] = new Material();
	materials["zoombieTextured"]->addTexture(GL_TEXTURE3, "Textures/zombie.png");
	materials["treeObjTextured"] = new Material();
	materials["treeObjTextured"]->addTexture(GL_TEXTURE4, "Textures/tree.png");


	activeScene = scene[1];
	GLuint image = SOIL_load_OGL_cubemap("Textures/posx.jpg", "Textures/negx.jpg", "Textures/posy.jpg", "Textures/negy.jpg", "Textures/posz.jpg", "Textures/negz.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	activeScene->skybox = new Skybox(GL_TEXTURE5, image, camera);
	activeScene->skybox->setTime(5);
	createForest();
	activeScene = scene[2];
	create3rdScene();
	//activeScene = scene[3];
	//create4thScene();
	
	activeScene = scene[0];
	activeScene->lights.at(1)->lightPosition = glm::vec3(0, 0, 1);
	activeScene->addSubjectToShader(activeScene->lights.at(1), shaders["textureShader"]);
	activeScene->addSubjectToShader(camera,  shaders["textureShader"]);
	auto obj = activeScene->addObject(models["triangleTextured"]
		, shaders["textureShader"]
	);
	obj->addMaterial(materials["grassTexture"]);


	obj->transformations->addTransform(std::make_unique<Translation>(-1,0,0));

	//obj->addTranslation(-1, 0, 0);

	auto obj1 = activeScene->addObject(models["triangleTextured"]
		, shaders["textureShader"]
	);
	obj1->addMaterial(materials["triangleTextured2"]);
	obj1->transformations->addTransform(std::make_unique<Translation>(1, 0, 0));
	//obj1->addTranslation(1, 0, 0);

	

}

void Application::run()
{
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glLoadIdentity();

	while (!glfwWindowShouldClose(window)) {
		// clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (activeScene->skybox)
		{
			activeScene->skybox->createSkybox();
			glClear(GL_DEPTH_BUFFER_BIT);
		}


		activeScene->renderScene();

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window);

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
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->lights.at(2)));
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->lights.at(3)));
	
	activeScene->shaders["textureShader"] = new ShaderProgram("textureVertex.txt", "textureFragment.txt");
	activeScene->shaders["DEFAULT"] = new ShaderProgram("default.vert.txt", "default.frag.txt");

	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->cam), activeScene->shaders["textureShader"]);
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->cam), activeScene->shaders["DEFAULT"]);

	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->light), activeScene->shaders["textureShader"]);
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->lights.at(1)), activeScene->shaders["textureShader"]);
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->lights.at(2)), activeScene->shaders["textureShader"]);
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->lights.at(3)), activeScene->shaders["textureShader"]);
	



	Model* treeModel = new Model();

	auto obj = activeScene->addObject(models["treeObj"], activeScene->shaders["textureShader"]);
	obj->addMaterial(materials["treeObjTextured"]);
	
	for (size_t i = 0; i < 50; i++)
	{
		float randomX = (static_cast<float>(rand()) / RAND_MAX) * 40- 20.f;
		float randomAngle = (static_cast<float>(rand()) / RAND_MAX)* 360 - 180.f;
		auto randomObject = activeScene->duplicateObject(obj);
		randomObject->transformations->addTransform(std::make_unique<Scaling>(0.1f));
		randomObject->transformations->addTransform(std::make_unique<Translation>(randomX, 0 , i));
		randomObject->transformations->addTransform(std::make_unique<Rotation>(randomAngle, 0, 1, 0));
		randomObject->transformations->addTransform(std::make_unique<Scaling>(0.2f));

	}
	obj->transformations->addTransform(std::make_unique<Scaling>(0.1f));
	obj->transformations->addTransform(std::make_unique<Scaling>(0.2f));

	obj = activeScene->addObject(models["bushes"]);
	
	for (size_t i = 0; i < 100; i++)
	{
		float randomX = (static_cast<float>(rand()) / RAND_MAX) * 20 - 10.f;
		float randomAngle = (static_cast<float>(rand()) / RAND_MAX) - 0.5f;

		auto randomObject = activeScene->duplicateObject(obj);
		randomObject->transformations->addTransform(std::make_unique<Scaling>(0.25f));
		randomObject->transformations->addTransform(std::make_unique<Translation>(randomX, 0, i*0.1f));
		randomObject->transformations->addTransform(std::make_unique<Rotation>(randomAngle, 0, 1, 0));
	}
	obj->transformations->addTransform(std::make_unique<Scaling>(0.25f));

	int random;
	for (auto& o : activeScene->objects) {
		random = rand() % 100;
		if (random < 25)
		{
			o->transformations->addTransform(std::make_unique<DynamicRotation>(glm::vec3(0, 1, 0), 0.01f));
		}
		
	}
	auto house = activeScene->addObject(models["house"], activeScene->shaders["textureShader"]);
	house->addMaterial(materials["houseTextured"]);
	house->transformations->addTransform(std::make_unique<Translation>(0, 0, 2));
	house->transformations->addTransform(std::make_unique<Scaling>(0.1f));

	/*
	auto car = activeScene->addObject(models["car"]);
	car->transformations->addTransform(std::make_unique<Translation>(-0.5f, 0, 3.6f));
	car->transformations->addTransform(std::make_unique<Rotation>(glm::radians(160.f), 0, 1, 0));
	car->transformations->addTransform(std::make_unique<Scaling>(0.2f));
	*/

	auto login = activeScene->addObject(models["login"], activeScene->shaders["DEFAULT"]);
	login->transformations->addTransform(std::make_unique<Translation>(-1.5f, 1, 0));

	auto zombie = activeScene->addObject(models["zoombi"], activeScene->shaders["textureShader"]);
	zombie->addMaterial(materials["zoombieTextured"]);
	zombie->transformations->addTransform(std::make_unique<Translation>(0, 0, 3.2f));
	zombie->transformations->addTransform(std::make_unique<Scaling>(0.1f));

	auto floor = activeScene->addObject(models["plainTextured"], activeScene->shaders["textureShader"]);


	floor->addMaterial(materials["grassTexture"]);
	floor->transformations->addTransform(std::make_unique<Scaling>(5));
	floor->transformations->addTransform(std::make_unique<Translation>(0,0,0.5f));
	floor->transformations->addTransform(std::make_unique<Rotation>(glm::radians(-90.f), 1, 0, 0));
}

void Application::create3rdScene()
{
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->light));
	activeScene->addSubjectToShader(dynamic_cast<Subject*>(activeScene->lights.at(0)));
	activeScene->light->lightPosition = glm::vec3(0,0,0);

	auto obj = activeScene->addObject(models["sphere"] );


	obj->transformations->addTransform(std::make_unique<Translation>(2, 0, 0));

	obj = activeScene->duplicateObject(obj);
	obj->transformations->addTransform(std::make_unique<Translation>(-2, 2, 0));
	

	obj = activeScene->duplicateObject(obj);
	obj->transformations->addTransform(std::make_unique<Translation>(-2, -2, 0));

	obj = activeScene->duplicateObject(obj);
	obj->transformations->addTransform(std::make_unique<Translation>(2, -2, 0));
}
/*
void Application::create4thScene()
{
	Light* light = activeScene->light;
	light->lightPosition = glm::vec3(0, 0, 0);
	//light->lightColor = glm::vec3(1, 0, 0);

	ShaderProgram* defaultShader = activeScene->shaders[0];
	defaultShader->addSubject(dynamic_cast<Subject*>(light));
	auto obj = activeScene->addObject(models["sphere"], defaultShader);
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
*/

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
	if (app->activeScene->skybox)
	{
		if (key == GLFW_KEY_V && action == GLFW_PRESS) {
			app->activeScene->skybox->isAttached = !app->activeScene->skybox->isAttached;
		}
	}
	
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
			app->isRightMousePressed = true;
		else if (action == GLFW_RELEASE)
			app->isRightMousePressed = false;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		GLbyte color[4];
		GLfloat depth;
		GLuint index; // identifikace tìlesa
		int newy = height - app->y - 10;
		glReadPixels(app->x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
		glReadPixels(app->x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		glReadPixels(app->x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
		printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth % f, stencil index % u\n", static_cast<int>(app->x), static_cast<int>(app->y), color[0], color[1], color[2], color[3], depth, index);


		//app->changeVisibleState(index);

		glm::vec3 screenX = glm::vec3(app->x, newy, depth);
		glm::mat4 view = app->camera->viewMatrix;
		glm::mat4 projection = app->camera->projectMatrix;
		glm::vec4 viewPort = glm::vec4(0, 0, width, height);
		glm::vec3 pos = glm::unProject(screenX, view, projection, viewPort);


		
		//obj->addTranslation(pos.x, pos.y,pos.z);
		//obj->addScaling(0.05f);

		app->bezierPositions[app->indexBezier] = pos;
		app->indexBezier++;
		if (app->indexBezier ==4)
		{
			glm::mat4x3 positions = {
				app->bezierPositions[0],
				app->bezierPositions[1],
				app->bezierPositions[2],
				app->bezierPositions[3],
			};

			//create bezier
			auto obj = app->activeScene->addObject(app->models["treeObj"], app->activeScene->shaders["textureShader"]);

			obj->addMaterial(app->materials["treeObjTextured"]);
			//obj->transformations->addTransform(std::make_unique<Translation>(pos.x, pos.y, pos.z));

			
			obj->transformations->addTransform(std::make_unique<BezierCurveTransform>(app->bezierMatrix, positions, 0.001f));
			obj->transformations->addTransform(std::make_unique<Scaling>(0.02f));
			app->indexBezier = 0;
		}
		
	}
}

void Application::changeVisibleState(GLuint index)
{
	activeScene->getObject(index)->isActive = !activeScene->getObject(index)->isActive;
}

void Application::cursor_callback(GLFWwindow* window, double xPos, double yPos)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	//printf("%f %f\n", xPos, yPos);
	app->x = xPos;
	app->y = yPos;
	//printf("%f %f\n", app->x, app->y);

	if (app->isRightMousePressed && app->activeScene->cam)
	{
		
		if (app->firstMouse)
		{
			app->lastX = xPos;
			app->lastY = yPos;
			app->firstMouse = false;
		}
		
		float xOffset = xPos - app->lastX;
		float yOffset = app->lastY - yPos;
		app->lastX = xPos;
		app->lastY = yPos;

		float sensitivity = 0.1f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		if (app->activeScene->cam)
		{
			app->activeScene->cam->rotate(xOffset, yOffset);
		}
	}
	else {
		app->firstMouse = true;
	}
}

void Application::window_size_callback(GLFWwindow* window, int width, int height)
{
	float ration = width / (float)height;
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	app->camera->projectMatrix = glm::perspective(glm::radians(90.f), ration, 0.1f, 100.f);
	glViewport(0, 0, width, height);
	app->camera->notifyAll();
}



