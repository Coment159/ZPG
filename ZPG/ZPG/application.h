#ifndef APPLICATION_H
#define APPLICATION_H
#include "shaderClass.h"
#include "model.h"
#include "scene.h"
#include "camera.h"
#include <vector>
#include "Models/sphere.h"
#include "Models/tree.h"
#include "Models/gift.h"
#include "Models/bushes.h"
#include "Models/triangle.h"
#include "Models/plain.h"
#include <unordered_map>
#include "Material.h"
#include "triangleT.h"
#include "plainT.h"

#include <array>

#include<assimp/Importer.hpp>// C++ importerinterface
#include<assimp/scene.h>// aiSceneoutputdata structure
#include<assimp/postprocess.h>// Post processingflags

#define GLM_ENABLE_EXPERIMENTAL
;

// Manages the whole "engine"
class Application
{
public:
	Application();
	~Application();
	void initialize();

	void createShaders();
	void createModels();
	void run();

	void downScale();
	void upScale();
	void treeRight();
	void treeLeft();
	void treeUp();
	void treeDown();
	void changeScene();
	void createForest();

	void create3rdScene();
	void create4thScene();

	static void error_callback(int error, const char* description) { fputs(description, stderr); }
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double xPos, double yPos);
	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

private:
	GLFWwindow* window = nullptr;
	bool firstMouse = true;

	float lastX = 400;
	float lastY = 300;
	float x = 0, y = 0;

	bool isRightMousePressed = false;

	const char* fragFile = "default.frag.txt";
	const char* vertFile = "default.vert.txt";

	std::vector<Scene*> scene;
	Scene* activeScene;
	int activeSceneInt;

	Application* instance;

	Camera* camera = new Camera();

	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, Material*> materials;
	std::unordered_map<std::string, ShaderProgram*> shaders;

	void changeVisibleState(GLuint index);

	glm::mat4 bezierMatrix = glm::mat4(
		glm::vec4(-1.0, 3.0, -3.0, 1.0),
		glm::vec4(3.0, -6.0, 3.0, 0.0),
		glm::vec4(-3.0, 3.0, 0.0, 0.0),
		glm::vec4(1.0, 0.0, 0.0, 0.0)
	);

	glm::vec3 bezierPositions[4];
	int indexBezier=0;
};


#endif // !APPLICATION_H
