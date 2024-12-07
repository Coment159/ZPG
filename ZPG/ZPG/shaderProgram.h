#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <vector>
#include "ShaderLoader.h"
#include "camera.h"
#include "Light.h"
#include "Material.h"

class Camera;
class ShaderProgram : ShaderLoader, public Observer
{
public:
	ShaderProgram(const char* vertFile, const char* fragFile);
	~ShaderProgram();


	void setModelMatrix(glm::mat4 M);
	void activate();
	void deactivate();

	void setMaterial(Material* material);

	void setTextureUnit(const char* location, GLuint value);

	//Observer
	void addSubject(Subject* sub);
	void updateCam(Camera* cam);
	void updateLight(Light* light);
	void notify(Subject* sub) override;
	void setFloat(const char* var, float value);

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

	Light* lights[10];
	int lightIndex = 0;

	const char* vertFile;
	const char* fragFile;
};


#endif // !SHADERPROGRAM_H


