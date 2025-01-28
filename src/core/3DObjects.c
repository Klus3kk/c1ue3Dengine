#include "3DObjects.h"
#include <glad/glad.h>  
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "Vectors.h"
#include "Camera.h"

#define PI 3.14159265358979323846

extern unsigned int shaderProgram;
// CUBE
void generateCubeVertices(float* vertices, unsigned int* indices, float size) {
    int vertexIndex = 0, index = 0;
    float halfSize = size / 2.0f;

    // Positions of the cube corners
    Vector3 positions[8] = {
        {-halfSize, -halfSize, -halfSize},
        { halfSize, -halfSize, -halfSize},
        { halfSize,  halfSize, -halfSize},
        {-halfSize,  halfSize, -halfSize},
        {-halfSize, -halfSize,  halfSize},
        { halfSize, -halfSize,  halfSize},
        { halfSize,  halfSize,  halfSize},
        {-halfSize,  halfSize,  halfSize}
    };

    // Texture coordinates for each vertex
    Vector2 texCoords[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    // Six faces, each with 4 vertices and 2 triangles
    int faces[6][4] = {
        {0, 1, 2, 3}, // Front face
        {5, 4, 7, 6}, // Back face
        {4, 0, 3, 7}, // Left face
        {1, 5, 6, 2}, // Right face
        {4, 5, 1, 0}, // Bottom face
        {3, 2, 6, 7}  // Top face
    };

    for (int i = 0; i < 6; i++) {
        // Generate vertices for each face
        for (int j = 0; j < 4; j++) {
            Vector3 pos = positions[faces[i][j]];
            Vector2 tex = texCoords[j];

            vertices[vertexIndex++] = pos.x;
            vertices[vertexIndex++] = pos.y;
            vertices[vertexIndex++] = pos.z;
            vertices[vertexIndex++] = tex.x;
            vertices[vertexIndex++] = tex.y;
        }

        // Two triangles per face
        int baseIndex = i * 4;
        indices[index++] = baseIndex;
        indices[index++] = baseIndex + 1;
        indices[index++] = baseIndex + 2;

        indices[index++] = baseIndex;
        indices[index++] = baseIndex + 2;
        indices[index++] = baseIndex + 3;
    }
}

Cube createCube(Vector3 position, Vector4 color, float size) {
    Cube cube;
    float* vertices = (float*)malloc(6 * 4 * 5 * sizeof(float)); // 6 faces, 4 vertices each, 5 floats per vertex
    unsigned int* indices = (unsigned int*)malloc(6 * 6 * sizeof(unsigned int)); // 6 faces, 6 indices each

    generateCubeVertices(vertices, indices, size);

    glGenVertexArrays(1, &cube.vao);
    glBindVertexArray(cube.vao);

    glGenBuffers(1, &cube.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * 5 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &cube.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    free(vertices);
    free(indices);

    cube.position = position;
    cube.color = color;
    return cube;
}


void drawCube(const Cube* cube, Matrix4x4 viewMatrix, Matrix4x4 projMatrix) {
    glUseProgram(shaderProgram);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    Matrix4x4 modelMatrix = translateMatrix(cube->position);  // Assuming translateMatrix is defined elsewhere

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix.data[0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix.data[0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMatrix.data[0]);

    // Set color
    GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
    glUniform4f(colorLoc, cube->color.x, cube->color.y, cube->color.z, cube->color.w);



    glBindVertexArray(cube->vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



void destroyCube(Cube* cube) {
    glDeleteVertexArrays(1, &cube->vao);
    glDeleteBuffers(1, &cube->vbo);
    glDeleteBuffers(1, &cube->ebo);
}


// CUBESPHERE (time to implement.: about 5 days :))
void generateSphereVertices(float* vertices, unsigned int* indices, float radius, int sectorCount, int stackCount) {
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    int vertexIndex = 0, index = 0;

    // Compute vertices
    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // The first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;           // from 0 to 2pi

            // Vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices[vertexIndex++] = x;
            vertices[vertexIndex++] = y;
            vertices[vertexIndex++] = z;

            // Normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices[vertexIndex++] = nx;
            vertices[vertexIndex++] = ny;
            vertices[vertexIndex++] = nz;

            // Vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices[vertexIndex++] = s;
            vertices[vertexIndex++] = t;
        }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices[index++] = k1;
                indices[index++] = k2;
                indices[index++] = k1 + 1;
            }

            if (i != (stackCount - 1)) {
                indices[index++] = k1 + 1;
                indices[index++] = k2;
                indices[index++] = k2 + 1;
            }
        }
    }
}

Sphere createSphere(float radius, int sectorCount, int stackCount, Vector3 position, Vector4 color) {
    Sphere sphere;
    int vertexCount = (stackCount + 1) * (sectorCount + 1);
    // 3 for position, 3 for normal, 2 for texture
    int numVertices = vertexCount * 8;
    int indexCount = stackCount * sectorCount * 6;

    float* vertices = (float*)malloc(numVertices * sizeof(float));
    unsigned int* indices = (unsigned int*)malloc(indexCount * sizeof(unsigned int));

    if (!vertices || !indices) {
        fprintf(stderr, "Failed to allocate memory for sphere.\n");
        exit(EXIT_FAILURE);
    }

    // Call the function to generate the vertices and indices for the sphere
    generateSphereVertices(vertices, indices, radius, sectorCount, stackCount);
    glGenVertexArrays(1, &sphere.vao);
    glBindVertexArray(sphere.vao);

    glGenBuffers(1, &sphere.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere.vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &sphere.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);

    free(vertices);
    free(indices);

    sphere.position = position;
    sphere.color = color;
    sphere.numVertices = vertexCount;
    sphere.numIndices = indexCount;

    return sphere;
}



// Function to draw a sphere
void drawSphere(const Sphere* sphere, Matrix4x4 viewMatrix, Matrix4x4 projMatrix) {
    glUseProgram(shaderProgram);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    Matrix4x4 modelMatrix = translateMatrix(sphere->position);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix.data[0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix.data[0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projMatrix.data[0]);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
    glUniform4f(colorLoc, sphere->color.x, sphere->color.y, sphere->color.z, sphere->color.w);

    glBindVertexArray(sphere->vao);
    glDrawElements(GL_TRIANGLES, sphere->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void destroySphere(Sphere* sphere) {
    glDeleteVertexArrays(1, &sphere->vao);
    glDeleteBuffers(1, &sphere->vbo);
    glDeleteBuffers(1, &sphere->ebo);
}

// PYRAMID
void generatePyramidVertices(float* vertices, unsigned int* indices, float baseSize, float height) {
    int vertexIndex = 0, index = 0;
    float halfSize = baseSize / 2.0f;

    // Base vertices
    Vector3 positions[5] = {
        {-halfSize, 0.0f, halfSize},   // Base - left front
        { halfSize, 0.0f, halfSize},   // Base - right front
        { halfSize, 0.0f, -halfSize},  // Base - right back
        {-halfSize, 0.0f, -halfSize},  // Base - left back
        {0.0f, height, 0.0f}           // Apex
    };

    // Texture coordinates for each vertex 
    Vector2 texCoords[5] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.5f, 0.5f} // Apex texture coordinate
    };

    // Vertices for base and sides
    for (int i = 0; i < 5; i++) {
        vertices[vertexIndex++] = positions[i].x;
        vertices[vertexIndex++] = positions[i].y;
        vertices[vertexIndex++] = positions[i].z;
        vertices[vertexIndex++] = texCoords[i].x;
        vertices[vertexIndex++] = texCoords[i].y;
    }

    // Indices for the base
    indices[index++] = 0;
    indices[index++] = 1;
    indices[index++] = 2;
    indices[index++] = 0;
    indices[index++] = 2;
    indices[index++] = 3;

    // Indices for the sides
    for (int i = 0; i < 4; i++) {
        indices[index++] = i;
        indices[index++] = (i + 1) % 4;
        indices[index++] = 4; // Apex index
    }
}

Pyramid createPyramid(Vector3 position, Vector4 color, float baseSize, float height) {
    Pyramid pyramid;
    float* vertices = (float*)malloc(5 * 5 * sizeof(float)); // 5 vertices, 5 floats per vertex
    unsigned int* indices = (unsigned int*)malloc(18 * sizeof(unsigned int)); // 6 indices for base, 12 for sides

    generatePyramidVertices(vertices, indices, baseSize, height);

    glGenVertexArrays(1, &pyramid.vao);
    glBindVertexArray(pyramid.vao);

    glGenBuffers(1, &pyramid.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pyramid.vbo);
    glBufferData(GL_ARRAY_BUFFER, 5 * 5 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &pyramid.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramid.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    free(vertices);
    free(indices);

    pyramid.position = position;
    pyramid.color = color;
    return pyramid;
}

// Function to draw a pyramid
void drawPyramid(const Pyramid* pyramid, Matrix4x4 viewMatrix, Matrix4x4 projMatrix) {
    glUseProgram(shaderProgram);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    // Create a translation matrix to place the pyramid correctly in the world
    Matrix4x4 translationMatrix = translateMatrix(pyramid->position);

    Matrix4x4 adjustmentMatrix = translateMatrix((Vector3) { 0.0f, -0.5f, 0.0f });
    Matrix4x4 modelMatrix = matrixMultiply(translationMatrix, adjustmentMatrix);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix.data[0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix.data[0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projMatrix.data[0]);

    // Set color
    GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
    glUniform4f(colorLoc, pyramid->color.x, pyramid->color.y, pyramid->color.z, pyramid->color.w);

    glBindVertexArray(pyramid->vao);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void destroyPyramid(Pyramid* pyramid) {
    glDeleteVertexArrays(1, &pyramid->vao);
    glDeleteBuffers(1, &pyramid->vbo);
    glDeleteBuffers(1, &pyramid->ebo);

}

// CYLINDER
void generateCylinderVertices(float* vertices, unsigned int* indices, float radius, float height, int sectorCount) {
    float angleStep = 2 * PI / sectorCount;
    float angle;
    int vertexIndex = 0, index = 0;

    // Top circle
    for (int i = 0; i <= sectorCount; ++i) {
        angle = i * angleStep;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        float y = height / 2;

        // Position
        vertices[vertexIndex++] = x;
        vertices[vertexIndex++] = y;
        vertices[vertexIndex++] = z;
        // Normal (pointing upwards for top circle)
        vertices[vertexIndex++] = 0.0f;
        vertices[vertexIndex++] = 1.0f;
        vertices[vertexIndex++] = 0.0f;
    }

    // Bottom circle
    for (int i = 0; i <= sectorCount; ++i) {
        angle = i * angleStep;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        float y = -height / 2;

        // Position
        vertices[vertexIndex++] = x;
        vertices[vertexIndex++] = y;
        vertices[vertexIndex++] = z;
        // Normal (pointing downwards for bottom circle)
        vertices[vertexIndex++] = 0.0f;
        vertices[vertexIndex++] = -1.0f;
        vertices[vertexIndex++] = 0.0f;
    }

    // Indices for the top circle
    for (int i = 0; i < sectorCount; ++i) {
        indices[index++] = i;
        indices[index++] = i + 1;
        indices[index++] = sectorCount;
    }

    // Indices for the bottom circle
    int bottomOffset = sectorCount + 1;
    for (int i = 0; i < sectorCount; ++i) {
        indices[index++] = bottomOffset + i;
        indices[index++] = bottomOffset + sectorCount;
        indices[index++] = bottomOffset + i + 1;
    }

    // Indices for the sides
    for (int i = 0; i < sectorCount; ++i) {
        int currentTop = i;
        int nextTop = i + 1;
        int currentBottom = bottomOffset + i;
        int nextBottom = bottomOffset + i + 1;

        // Triangle 1
        indices[index++] = currentTop;
        indices[index++] = currentBottom;
        indices[index++] = nextBottom;

        // Triangle 2
        indices[index++] = currentTop;
        indices[index++] = nextBottom;
        indices[index++] = nextTop;
    }
}

Cylinder createCylinder(float radius, float height, int sectorCount, Vector3 position, Vector4 color) {
    Cylinder cylinder;
    int vertexCount = (sectorCount + 1) * 2 + 2; // +2 for center points of top and bottom circle
    int numVertices = vertexCount * 6; // 3 for position, 3 for normal
    int indexCount = sectorCount * 12; // 6 indices per sector for sides, top and bottom

    float* vertices = (float*)malloc(numVertices * sizeof(float));
    unsigned int* indices = (unsigned int*)malloc(indexCount * sizeof(unsigned int));

    if (!vertices || !indices) {
        fprintf(stderr, "Failed to allocate memory for cylinder.\n");
        exit(EXIT_FAILURE);
    }

    generateCylinderVertices(vertices, indices, radius, height, sectorCount);

    glGenVertexArrays(1, &cylinder.vao);
    glBindVertexArray(cylinder.vao);

    glGenBuffers(1, &cylinder.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cylinder.vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &cylinder.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinder.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    // Cleanup
    free(vertices);
    free(indices);

    cylinder.position = position;
    cylinder.color = color;
    cylinder.radius = radius;
    cylinder.height = height;
    cylinder.sectorCount = sectorCount;

    return cylinder;
}

void drawCylinder(const Cylinder* cylinder, Matrix4x4 viewMatrix, Matrix4x4 projMatrix) {
    glUseProgram(shaderProgram);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    Matrix4x4 modelMatrix = translateMatrix(cylinder->position); 

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix.data[0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix.data[0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMatrix.data[0]);

    // Set color
    GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
    glUniform4f(colorLoc, cylinder->color.x, cylinder->color.y, cylinder->color.z, cylinder->color.w);

    glBindVertexArray(cylinder->vao);
    glDrawElements(GL_TRIANGLES, cylinder->sectorCount * 12, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void destroyCylinder(Cylinder* cylinder) {
    glDeleteVertexArrays(1, &cylinder->vao);
    glDeleteBuffers(1, &cylinder->vbo);
    glDeleteBuffers(1, &cylinder->ebo);
}

// PLANE 

Plane createPlane(Vector3 position, Vector4 color) {
    Plane plane;
    float halfWidth = 150.0f;
    float halfHeight = 150.0f;
    float vertices[] = {
        // Position                // Texture Coords
        -halfWidth, 0.0f,  halfHeight,  0.0f, 1.0f, // Top-left
         halfWidth, 0.0f,  halfHeight,  1.0f, 1.0f, // Top-right
         halfWidth, 0.0f, -halfHeight,  1.0f, 0.0f, // Bottom-right
        -halfWidth, 0.0f, -halfHeight,  0.0f, 0.0f  // Bottom-left
    };
    unsigned int indices[] = {
        0, 1, 2, // First Triangle
        0, 2, 3  // Second Triangle
    };

    glGenVertexArrays(1, &plane.vao);
    glBindVertexArray(plane.vao);

    glGenBuffers(1, &plane.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, plane.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &plane.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    plane.position = position;
    plane.color = color;
    return plane;
}

void drawPlane(const Plane* plane, Matrix4x4 viewMatrix, Matrix4x4 projMatrix) {
    glUseProgram(shaderProgram);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    Matrix4x4 modelMatrix = translateMatrix(plane->position);  

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix.data[0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix.data[0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMatrix.data[0]);

    // Set color
    GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
    glUniform4f(colorLoc, plane->color.x, plane->color.y, plane->color.z, plane->color.w);

    glBindVertexArray(plane->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void destroyPlane(Plane* plane) {
    glDeleteVertexArrays(1, &plane->vao);
    glDeleteBuffers(1, &plane->vbo);
    glDeleteBuffers(1, &plane->ebo);
}
