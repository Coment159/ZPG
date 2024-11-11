#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <vector>
#include "ShaderLoader.h"
#include "camera.h"
#include "Light.h"

class Camera;
class ShaderProgram : ShaderLoader, public Observer
{
public:
	ShaderProgram(const char* vertFile, const char* fragFile);
	~ShaderProgram();


	void setModelMatrix(glm::mat4 M);
	void activate();



	//Observer
	void addSubject(Subject* sub);
	void updateCam(Camera* cam);
	void updateLight(Light* light);
	void notify(Subject* sub) override;

private:
	void setVec3(const char* vector, glm::vec3 values);
	int findLight(Light* light);

	GLuint modelMatrixId;
	GLuint viewMatrixId;
	GLuint projectMatrixId;
	//GLuint lightPositionId;
	GLuint ambientId;

	GLuint numberOfLightsLID;
	GLuint lightsPositionID;
	GLuint lightsColorID;


	//Camera* cam = nullptr;
	//Light* light = nullptr;

	Light* lights[4];
	int lightIndex = 0;

	const char* vertFile;
	const char* fragFile;
};


#endif // !SHADERPROGRAM_H


