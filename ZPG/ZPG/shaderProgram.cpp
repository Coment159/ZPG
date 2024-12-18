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

}

ShaderProgram::~ShaderProgram()
{
	this->deleteShader();
}
void ShaderProgram::activate()
{
	glUseProgram(this->shaderProgramID);
}

void ShaderProgram::deactivate()
{
	glUseProgram(0);
}

void ShaderProgram::setMaterial(Material* material)
{
	setVec3("material.diffColor", material->diffColor);
	setVec3("material.specColor", material->specColor);
	setVec3("material.ambientColor", material->ambientColor);
	setFloat("material.shine", material->shine);

	GLuint textureId = glGetUniformLocation(shaderProgramID, "textureUnitID");
	if ((textureId != 0) && (material->basicTexture != -1))
	{
		glUniform1i(textureId, material->basicTexture);
	}
}

void ShaderProgram::setTextureUnit(const char* location, GLuint value)
{
	glUseProgram(shaderProgramID);
	GLuint id = glGetUniformLocation(shaderProgramID, location);
	glUniform1i(id, value);
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
	id = glGetUniformLocation(shaderProgramID, ("lights[" + std::to_string(index) + "].type").c_str());
	glUniform3fv(id, 1, glm::value_ptr(light->getType()));
	id = glGetUniformLocation(shaderProgramID, ("lights[" + std::to_string(index) + "].lightDirection").c_str());
	glUniform3fv(id, 1, glm::value_ptr(light->lightDirection));
	id = glGetUniformLocation(shaderProgramID, ("lights[" + std::to_string(index) + "].cutOff").c_str());
	glUniform1f(id, glm::cos(glm::radians(light->cutOff)));

	//std::cout << light->cutOff << std::endl;

	
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
	GLuint id = glGetUniformLocation(this->shaderProgramID, vector);
	if (id != 0)
	{
		glUniform3f(id, values.x, values.y, values.z);
	}
	
}

void ShaderProgram::setFloat(const char* var, float value)
{
	GLuint id = glGetUniformLocation(shaderProgramID, var);
	if (id != 0)
	{
		glUniform1f(id, value);
	}

	
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
