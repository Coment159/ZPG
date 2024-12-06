#include "Material.h"

Material::Material()
{
	diffColor = glm::vec3(1, 1, 1);
	specColor = glm::vec3(1, 1, 1);
	ambientColor = glm::vec3(1, 1, 1);
	shine = 32.f;
	basicTexture = -1;
}

void Material::addTexture(GLenum gl_texture, const char* texture_path)
{
	glActiveTexture(gl_texture);
	GLuint textureID = SOIL_load_OGL_texture(texture_path, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, textureID);
	basicTexture = gl_texture - GL_TEXTURE0;
}
