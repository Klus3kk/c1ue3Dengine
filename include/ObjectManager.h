#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <stdbool.h>
#include "Camera.h"
#include "3DObjects.h"
#include "materials.h"
#include "Vectors.h"
#include "SceneObject.h"

#define MAX_OBJECTS 1000 

typedef struct {
    SceneObject objects[MAX_OBJECTS];
    int count;
    int capacity;
} ObjectManager;

extern ObjectManager objectManager;

void initObjectManager();
void addObjectToManager(SceneObject newObject);
void addObject(Camera* camera, ObjectType type, bool useTexture, int textureIndex, bool colorCreation, Model* model, PBRMaterial material, bool usePBR);
void removeObject(int index);
void cleanupObjects();
void updateObjectInManager(SceneObject* updatedObject);
void drawObject(const SceneObject* obj, const Matrix4x4 viewMatrix, const Matrix4x4 projMatrix);

#endif 