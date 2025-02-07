#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "types.h"
#include "Vectors.h"
#include "materials.h"
#include "Object3D.h"

typedef struct SceneObject {
    Object3D object;  // Base object
    Vector3 position; // Position of the object
    Vector3 rotation; // Rotation of the object
    Vector3 scale;    // Scale of the object
    Vector4 color;    // Color of the object
    bool selected;    // Selection flag
    int id;           // Unique ID
} SceneObject;

#endif 
