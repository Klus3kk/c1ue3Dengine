#include "ModelLoad.h"

Mesh processMesh(struct aiMesh* mesh, const struct aiScene* scene) {
    Mesh newMesh = { 0 };
    if (!mesh) return newMesh;

    glGenVertexArrays(1, &newMesh.VAO);
    glGenBuffers(1, &newMesh.VBO);
    glGenBuffers(1, &newMesh.EBO);

    glBindVertexArray(newMesh.VAO);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(struct aiVector3D), mesh->mVertices, GL_STATIC_DRAW);

    // Indices
    newMesh.indices = malloc(mesh->mNumFaces * 3 * sizeof(unsigned int));
    if (!newMesh.indices) {
        fprintf(stderr, "Failed to allocate memory for indices.\n");
        glDeleteVertexArrays(1, &newMesh.VAO);
        glDeleteBuffers(1, &newMesh.VBO);
        glDeleteBuffers(1, &newMesh.EBO);
        return newMesh;
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            newMesh.indices[i * mesh->mFaces[i].mNumIndices + j] = mesh->mFaces[i].mIndices[j];
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mNumFaces * 3 * sizeof(unsigned int), newMesh.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct aiVector3D), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);  // Unbind VAO

    newMesh.numVertices = mesh->mNumVertices;
    newMesh.numIndices = mesh->mNumFaces * 3;
    return newMesh;
}


Model* loadModel(const char* path) {
    const struct aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Failed to load model: %s\n", aiGetErrorString());
        return NULL;
    }

    if (scene->mNumMeshes == 0) {
        fprintf(stderr, "No meshes found in the model.\n");
        aiReleaseImport(scene);
        return NULL;
    }

    Model* model = (Model*)malloc(sizeof(Model));
    if (!model) {
        fprintf(stderr, "Failed to allocate memory for the model.\n");
        aiReleaseImport(scene);
        return NULL;
    }

    strncpy(model->path, path, sizeof(model->path) - 1);
    model->path[sizeof(model->path) - 1] = '\0';
    model->meshCount = scene->mNumMeshes;
    model->meshes = (Mesh*)malloc(model->meshCount * sizeof(Mesh));
    if (!model->meshes) {
        fprintf(stderr, "Failed to allocate memory for meshes.\n");
        aiReleaseImport(scene);
        free(model);
        return NULL;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        model->meshes[i] = processMesh(scene->mMeshes[i], scene);
    }

    aiReleaseImport(scene);
    return model;
}


void freeModel(Model* model) {
    if (!model) return;

    for (unsigned int i = 0; i < model->meshCount; i++) {
        Mesh* mesh = &model->meshes[i];

        if (mesh->VAO) {
            glDeleteVertexArrays(1, &mesh->VAO);
            mesh->VAO = 0;
        }
        if (mesh->VBO) {
            glDeleteBuffers(1, &mesh->VBO);
            mesh->VBO = 0;
        }
        if (mesh->EBO) {
            glDeleteBuffers(1, &mesh->EBO);
            mesh->EBO = 0;
        }

        if (mesh->indices) {
            free(mesh->indices);
            mesh->indices = NULL;
        }
    }

    if (model->meshes) {
        free(model->meshes);
        model->meshes = NULL;
    }
}
