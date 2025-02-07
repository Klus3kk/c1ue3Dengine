#pragma once
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "Vectors.h"
typedef struct {
    GLuint vao; // Vertex Array Object ID
    GLuint vbo; // Vertex Buffer Object ID
    GLuint ebo; // Element Buffer Object ID
    Vector3 position; // Position of the cube
    Vector4 color;     // Color of the cube
} Cube;


typedef struct {
    float radius;
    int sectorCount;
    int stackCount;
    bool smooth;
} SphereSettings;

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    Vector3 position;
    Vector4 color;
    SphereSettings settings;  
    int numVertices;
    int numIndices;
} Sphere;

typedef struct {
    GLuint vao; 
    GLuint vbo; 
    GLuint ebo; 
    Vector3 position; 
    Vector4 color;    
} Pyramid;

typedef struct {
    GLuint vao; // Vertex Array Object ID
    GLuint vbo; // Vertex Buffer Object ID
    GLuint ebo; // Element Buffer Object ID
    Vector3 position; // Position of the cube
    Vector4 color;     // Color of the cube
    float radius;
    float height;
    int sectorCount;
} Cylinder;

typedef struct {
    GLuint vao; // Vertex Array Object ID
    GLuint vbo; // Vertex Buffer Object ID
    GLuint ebo; // Element Buffer Object ID
    Vector3 position; // Position of the plane
    Vector4 color;    // Color of the plane
} Plane;


// Cube
Cube createCube(Vector3 position, Vector4 color, float size);
void drawCube(const Cube* cube, Matrix4x4 viewMatrix, Matrix4x4 projMatrix);
void destroyCube(Cube* cube);

// Sphere
Sphere createSphere(float radius, int sectorCount, int stackCount, Vector3 position, Vector4 color);
void drawSphere(const Sphere* sphere, Matrix4x4 viewMatrix, Matrix4x4 projMatrix);
void destroySphere(Sphere* sphere);

// Pyramid
Pyramid createPyramid(Vector3 position, Vector4 color, float baseSize, float height);
void drawPyramid(const Pyramid* pyramid, Matrix4x4 viewMatrix, Matrix4x4 projMatrix);
void destroyPyramid(Pyramid* pyramid);

// Cylinder
Cylinder createCylinder(float radius, float height, int sectorCount, Vector3 position, Vector4 color);
void drawCylinder(const Cylinder* cylinder, Matrix4x4 viewMatrix, Matrix4x4 projMatrix);
void destroyCylinder(Cylinder* cylinder);

// Plane
Plane createPlane(Vector3 position, Vector4 color);
void drawPlane(const Plane* plane, Matrix4x4 viewMatrix, Matrix4x4 projMatrix);
void destroyPlane(Plane* plane);