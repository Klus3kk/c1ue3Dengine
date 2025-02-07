#ifndef MODELLOAD_H
#define MODELLOAD_H

#include "Vectors.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    Vertex* vertices;
    unsigned int* indices;
    unsigned int numVertices;
    unsigned int numIndices;
} Mesh;

typedef struct {
    Mesh* meshes;
    unsigned int meshCount;
    char path[256];
} Model;

Mesh processMesh(struct aiMesh* mesh, const struct aiScene* scene);
Model* loadModel(const char* path);
void freeModel(Model* model);

#endif 
