#ifndef MATERIALS_H
#define MATERIALS_H

#include <glad/glad.h>  
#include <GLFW/glfw3.h>
typedef struct {
    GLuint albedoMap;
    GLuint normalMap;
    GLuint metallicMap;
    GLuint roughnessMap;
    GLuint aoMap;
} PBRMaterial;

#define MAX_MATERIALS 50  

// Material storage 
extern PBRMaterial materials[MAX_MATERIALS];
extern const char* materialNames[MAX_MATERIALS];
extern int materialCount;


PBRMaterial loadPBRMaterial(const char* albedo, const char* normal, const char* metallic, const char* roughness, const char* ao);
void bindPBRMaterial(PBRMaterial material);
void cleanupPBRMaterial(PBRMaterial* material);
void addMaterial(const char* name, PBRMaterial material);
PBRMaterial* getMaterial(const char* name);

#endif 
