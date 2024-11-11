#include "shaderClass.h"

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragSource = fragCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint infoLogLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::string infoLog(infoLogLength, ' ');
		glGetShaderInfoLog(vertexShader, infoLogLength, NULL, &infoLog[0]);
		std::cerr << "Vertex shader compilation error: " << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint infoLogLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::string infoLog(infoLogLength, ' ');
		glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, &infoLog[0]);
		std::cerr << "Fragment shader compilation error: " << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, fragmentShader);
	glAttachShader(ID, vertexShader);
	glLinkProgram(ID);


	GLint status;
	glGetProgramiv(ID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(ID, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	activate();

	modelMatrixId = glGetUniformLocation(ID, "modelMatrix");
	if (modelMatrixId != 0)
	{
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
	}
	viewMatrixId = glGetUniformLocation(ID, "viewMatrix");	
	if (viewMatrixId != 0)
	{
		glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
	}
	projectMatrixId = glGetUniformLocation(ID, "projectMatrix");
	if (projectMatrixId != 0)
	{
		glUniformMatrix4fv(projectMatrixId, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
	}
}
void Shader::addCamera(Camera* cam)
{
	this->cam = cam;
	//cam->addObserver(this);
	update();
}

void Shader::setUniformMatrix(glm::mat4& M)
{
	glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &M[0][0]);
}
void Shader::update()
{
	activate();
	glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &cam->viewMatrix[0][0]);
	glUniformMatrix4fv(projectMatrixId, 1, GL_FALSE, &cam->projectMatrix[0][0]);
}
void Shader::activate() {
	glUseProgram(ID);
}
void Shader::deactivate() {
	glDeleteProgram(ID);
}