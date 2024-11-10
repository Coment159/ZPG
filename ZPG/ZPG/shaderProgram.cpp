#include "shaderProgram.h"


ShaderProgram::ShaderProgram(const char* vertFile, const char* fragFile)
{
	this->vertFile = vertFile, this->fragFile = fragFile;
	this->loadShader(vertFile, fragFile);
	activate();
	

	modelMatrixId = glGetUniformLocation(this->shaderProgramID, "modelMatrix");
	if (modelMatrixId != 0)
	{
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
	}
	viewMatrixId = glGetUniformLocation(this->shaderProgramID, "viewMatrix");
	if (viewMatrixId != 0)
	{
		glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
	}
	projectMatrixId = glGetUniformLocation(this->shaderProgramID, "projectMatrix");
	if (projectMatrixId != 0)
	{
		glUniformMatrix4fv(projectMatrixId, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
	}
	numberOfLightsLID = glGetUniformLocation(shaderProgramID, "numberOfLights");
	lightsPositionID = glGetUniformLocation(shaderProgramID, "lights[0].position");
	lightsColorID = glGetUniformLocation(shaderProgramID, "lights[0].color");

	glUniform1i(numberOfLightsLID, 0);
	/*
	lightPositionId = glGetUniformLocation(this->shaderProgramID, "lightPosition");
	if (lightPositionId != -1 )
	{
		glUniform3f(lightPositionId, 5, 5, 5.0);
	}
	*/
	/*
	ambientId = glGetUniformLocation(this->shaderProgramID, "ambient");
	if (ambientId != -1)
	{
		glUniform3f(ambientId, 0.1, 0.1, 0.1);
	}
	*/
	/*
	GLuint lightColorId = glGetUniformLocation(this->shaderProgramID, "lightColor");
	if (lightColorId != -1)
	{
		glUniform3f(lightColorId, 1.0, 1.0, 1.0);
	}
	*/
}

ShaderProgram::~ShaderProgram()
{
	this->deleteShader();
}
void ShaderProgram::activate()
{
	glUseProgram(this->shaderProgramID);
}

void ShaderProgram::addSubject(Subject* sub)
{
	sub->attach(this);
	if (Light* light = dynamic_cast<Light*> (sub))
	{
		lights[lightIndex] = light;
		lightIndex++;
		activate();
		glUniform1i(numberOfLightsLID, lightIndex);
	}
	notify(sub);
}

void ShaderProgram::updateCam(Camera* cam)
{

	glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &cam->viewMatrix[0][0]);
	glUniformMatrix4fv(projectMatrixId, 1, GL_FALSE, &cam->projectMatrix[0][0]);
	setVec3("viewPos", cam->position);
}

void ShaderProgram::updateLight(Light* light)
{
	int index = findLight(light);

	GLuint id = glGetUniformLocation(shaderProgramID, ("lights[" + std::to_string(index) + "].position").c_str());
	glUniform3fv(id, 1, glm::value_ptr(light->getPosition()));
	id = glGetUniformLocation(shaderProgramID, ("lights[" + std::to_string(index) + "].color").c_str());
	glUniform3fv(id, 1, glm::value_ptr(light->lightColor));


	//setVec3("lightPosition", light->lightPosition);
	//setVec3("lightColor", light->lightColor);
}

void ShaderProgram::notify(Subject* sub) 
{
	activate();
	if (Camera* cam = dynamic_cast<Camera*>(sub))
	{
		updateCam(cam);
	}
	if (Light* light = dynamic_cast<Light*>(sub))
	{
		updateLight(light);
	}
	
}

void ShaderProgram::setVec3(const char* vector, glm::vec3 values)
{
	glUniform3f(glGetUniformLocation(this->shaderProgramID, vector), values.x, values.y, values.z);
}

int ShaderProgram::findLight(Light* light)
{
	for (int i = 0; i < lightIndex; i++)
	{
		if (lights[i] == light) {
			return i;
		}
	}
}


void ShaderProgram::setModelMatrix(glm::mat4 M)
{
	glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &M[0][0]);
}
