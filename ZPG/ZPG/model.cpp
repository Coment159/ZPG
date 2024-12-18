#include "model.h"

Model::Model(const float verticies[], GLuint size, bool hasTexture) : vao(hasTexture? new VAOT() : new VAO()), vbo(new VBO(verticies, size, hasTexture ? 8 : 6)) {

	vao->LinkVBO(vbo, 0);
}

Model::Model(std::string path,  bool index)
{
    Assimp::Importer importer;
    unsigned int importOptions = 0;
    if (index)
    {
        importOptions = aiProcess_Triangulate //Converts polygons to triangles
            | aiProcess_OptimizeMeshes              // Reduces the number of submeshes
            | aiProcess_JoinIdenticalVertices       // Removes duplicate vertices
            //| aiProcess_CalcTangentSpace;           // Computes tangents and bitangents
            ;
    }

    //aiProcess_GenNormals/ai_Process_GenSmoothNormals - Generates flat/Smooth normals

    const aiScene* scene = importer.ReadFile(path, importOptions);
    if (scene)
    {
        printf("scene->mNumMeshes = %d\n", scene->mNumMeshes);
        printf("scene->mNumMaterials = %d\n", scene->mNumMaterials);

        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            Vertex* pVertices = new Vertex[mesh->mNumVertices];
            std::memset(pVertices, 0, sizeof(Vertex) * mesh->mNumVertices);
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                if (mesh->HasPositions()) {
                    pVertices[i].Position[0] = mesh->mVertices[i].x;
                    pVertices[i].Position[1] = mesh->mVertices[i].y;
                    pVertices[i].Position[2] = mesh->mVertices[i].z;
                }

                if (mesh->HasNormals()) {
                    pVertices[i].Normal[0] = mesh->mNormals[i].x;
                    pVertices[i].Normal[1] = mesh->mNormals[i].y;
                    pVertices[i].Normal[2] = mesh->mNormals[i].z;
                }

                if (mesh->HasTextureCoords(0)) {
                    pVertices[i].Texture[0] = mesh->mTextureCoords[0][i].x;
                    pVertices[i].Texture[1] = mesh->mTextureCoords[0][i].y;
                }
                /*
                if (mesh->HasTangentsAndBitangents()) {
                    pVertices[i].Tangent[0] = mesh->mTangents[i].x;
                    pVertices[i].Tangent[1] = mesh->mTangents[i].y;
                    pVertices[i].Tangent[2] = mesh->mTangents[i].z;
                }
                */
            }
            unsigned int* pIndices = nullptr;
            if (mesh->HasFaces()) {
                pIndices = new unsigned int[mesh->mNumFaces * 3];
                for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                    pIndices[i * 3] = mesh->mFaces[i].mIndices[0];
                    pIndices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
                    pIndices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
                }
            }

            vao = new VAOT();
            vbo = new VBO(pVertices, mesh->mNumVertices);
            dynamic_cast<VAOT*>(vao)->LinkVBOVertex(vbo);
            if (index)
            {
                glGenBuffers(1, &ibo);
                vao->Bind();
                vbo->Bind();
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * 3, pIndices, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                vao->Bind();

                indiciesCount = mesh->mNumFaces * 3;
                delete[] pVertices;
                delete[] pIndices;
            }

        }
        vao->Unbind();

    }
}

void Model::activate()
{
	vao->Bind();
}

void Model::deactivate() {
	vao->Unbind();
}

