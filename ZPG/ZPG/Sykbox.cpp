#include "Sykbox.h"

Skybox::Skybox(GLenum glText, GLuint image, Camera* cam)
{
    cam->attach(this);
    //Vertex Array Object (VAO)
    VBO = 0;
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float) * 108, skycube, GL_STATIC_DRAW);

    VAO = 0;
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

    //shader = new ShaderProgram("Shaders/SkyboxVertex.txt", "Shaders/SkyboxFragment.txt");
    shader = new ShaderProgram("default.vert.txt", "Shaders/SkyboxFragment.txt");
    glActiveTexture(glText);

    //glBindTexture(GL_TEXTURE_CUBE_MAP, image);
    glBindTexture(GL_TEXTURE_CUBE_MAP, image);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


    textureId = glText - GL_TEXTURE0;

    shader->addSubject(cam);

}

void Skybox::createSkybox()
{
    // Activate shader
    shader->activate();
    shader->setTextureUnit("UISky", textureId);
   
    //model = glm::translate(model, glm::vec3(0,0,0));
    shader->setModelMatrix(model);

    glBindVertexArray(VAO); // Bind the skybox VAO
    glDrawArrays(GL_TRIANGLES, 0, 108); // Draw the skybox
    glBindVertexArray(0);

}

void Skybox::notify(Subject* sub)
{
    Camera* cam = dynamic_cast<Camera*>(sub);
    if (cam && isAttached)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cam->position);
    }
}
