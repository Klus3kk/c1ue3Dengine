#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Vectors.h"
#include "Camera.h"
#include "SOIL2/SOIL2.h"
extern Camera camera;  // If the camera is globally accessible
extern Matrix4x4 projMatrix;  // If the projMatrix is globally accessible
extern const char* backgroundNames[];
extern const int backgroundCount;
void initSkybox(int skyboxIndex);
void drawSkybox(const Camera* camera, const Matrix4x4* projMatrix);
GLuint loadCubemap(const char* faceFiles[6]);

#endif
