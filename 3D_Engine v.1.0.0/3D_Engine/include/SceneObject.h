#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "Object3D.h"
#include "Vectors.h"

typedef struct {
    Object3D object; // Object3D contains the basic properties like type, data, etc.
    Vector3 position; // Position of the object
    Vector3 rotation; // Rotation of the object
    Vector3 scale; // Scale of the object
    Vector4 color; // Color of the object
    bool selected; // If the object is selected
    int id; // Unique identifier for the object
} SceneObject;


#endif // SCENE_OBJECT_H
