#ifndef TEXTURES_H
#define TEXTURES_H

#include <glad/glad.h>

extern const char* textureNames[];
extern int textureCount; // Remove 'const' to match with the definition in 'textures.c'
#define MAX_TEXTURES 10  // Adjust based on how many textures you plan to use
extern GLuint textures[MAX_TEXTURES];  // Array to store texture IDs

GLuint loadTexture(const char* path);
void loadAllTextures();
void loadPBRTextures();

#endif // TEXTURES_H