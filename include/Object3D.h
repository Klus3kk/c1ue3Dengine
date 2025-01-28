#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Vectors.h"
#include "materials.h"
#include "3DObjects.h"
#include "ModelLoad.h"

typedef enum {
    OBJ_CUBE,
    OBJ_SPHERE,
    OBJ_PYRAMID,
    OBJ_CYLINDER,
    OBJ_PLANE,
    OBJ_MODEL
} ObjectType;

typedef struct {
    char name[128];
    ObjectType type;
    union {
        Cube cube;
        Sphere sphere;
        Pyramid pyramid;
        Cylinder cylinder;
        Plane plane;
        Model model; 
    } data;
    int textureID;
    bool useTexture;
    bool useColor;
    bool useLighting;
    PBRMaterial material;
    bool usePBR;
} Object3D;

#endif 
