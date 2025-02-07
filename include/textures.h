#ifndef TEXTURES_H
#define TEXTURES_H

#include <glad/glad.h>  
#include <GLFW/glfw3.h>

extern const char* textureNames[];
extern int textureCount; 
#define MAX_TEXTURES 10  // Adjust based on how many textures you plan to use
extern GLuint textures[MAX_TEXTURES];  // Array to store texture IDs

GLuint loadTexture(const char* path);
void loadAllTextures();
void loadPBRTextures();
GLuint getTexture(const char* name);

#endif 