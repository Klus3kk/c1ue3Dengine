#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "globals.h"
#include "ModelLoad.h"
#include "rendering.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Vectors.h"
#include "materials.h"
#include "textures.h"
#include "gui.h"
#include "SceneObject.h"
#include "Object3D.h"

ObjectManager objectManager;

void initObjectManager() {
    objectManager.count = 0;
    objectManager.capacity = MAX_OBJECTS;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        objectManager.objects[i].id = -1; // Initialize object IDs to -1 to indicate they are not used
    }
}

void addObjectToManager(SceneObject newObject) {
    static int currentID = 0; // Static variable to keep track of unique IDs
    if (objectManager.count < MAX_OBJECTS) {
        newObject.id = currentID++; // Assign a unique ID to the new object
        objectManager.objects[objectManager.count++] = newObject;
    }
}

void addObject(Camera* camera, ObjectType type, bool useTexture, int textureIndex, bool colorCreation, Model* model, PBRMaterial material, bool usePBR) {
    if (objectManager.count >= MAX_OBJECTS) return;

    SceneObject newObject;
    newObject.object.type = type;
    newObject.object.useTexture = useTexture;
    newObject.object.textureID = textureIndex;
    newObject.object.useColor = colorCreation;
    newObject.object.useLighting = true;
    newObject.object.usePBR = usePBR;

    if (!useTexture && !usePBR) {
        newObject.object.useColor = true;
    }

    newObject.object.material = material;
    newObject.object.textureID = useTexture ? getTexture(textureNames[textureIndex]) : 0;

    newObject.position = vector_add(camera->Position, vector_scale(camera->Front, 5.0f)); // Position in front of the camera
    newObject.rotation = (Vector3){ 0.0f, 0.0f, 0.0f };
    newObject.scale = (Vector3){ 1.0f, 1.0f, 1.0f };
    newObject.color = (Vector4){ 1.0f, 1.0f, 1.0f, 1.0f }; // Default to white color
    newObject.selected = false;

    switch (type) {
    case OBJ_CUBE:
        newObject.object.data.cube = createCube(newObject.position, newObject.color, 1.0f);
        break;
    case OBJ_SPHERE:
        newObject.object.data.sphere = createSphere(1.0f, 20, 20, newObject.position, newObject.color);
        break;
    case OBJ_PYRAMID:
        newObject.object.data.pyramid = createPyramid(newObject.position, newObject.color, 1.0f, 1.0f);
        break;
    case OBJ_CYLINDER:
        newObject.object.data.cylinder = createCylinder(1.0f, 2.0f, 20, newObject.position, newObject.color);
        break;
    case OBJ_PLANE:
        newObject.object.data.plane = createPlane(newObject.position, newObject.color);
        break;
    case OBJ_MODEL:
        if (model) {
            newObject.object.data.model = *model;
            newObject.object.data.model.meshes = (Mesh*)malloc(model->meshCount * sizeof(Mesh));
            for (unsigned int i = 0; i < model->meshCount; i++) {
                newObject.object.data.model.meshes[i] = model->meshes[i];
            }
            newObject.object.data.model.meshCount = model->meshCount;
        }
        break;
    }

    addObjectToManager(newObject);
}

void removeObject(int index) {
    if (index < 0 || index >= objectManager.count) {
        printf("Invalid index: %d\n", index);
        return;
    }

    printf("Removing object at index: %d\n", index);

    SceneObject* obj = &objectManager.objects[index];

    switch (obj->object.type) {
    case OBJ_CUBE:
        printf("Destroying cube at index: %d\n", index);
        destroyCube(&obj->object.data.cube);
        break;
    case OBJ_SPHERE:
        printf("Destroying sphere at index: %d\n", index);
        destroySphere(&obj->object.data.sphere);
        break;
    case OBJ_PYRAMID:
        printf("Destroying pyramid at index: %d\n", index);
        destroyPyramid(&obj->object.data.pyramid);
        break;
    case OBJ_CYLINDER:
        printf("Destroying cylinder at index: %d\n", index);
        destroyCylinder(&obj->object.data.cylinder);
        break;
    case OBJ_PLANE:
        printf("Destroying plane at index: %d\n", index);
        destroyPlane(&obj->object.data.plane);
        break;
    case OBJ_MODEL:
        printf("Freeing model at index: %d\n", index);
        freeModel(&obj->object.data.model);
        break;
    default:
        printf("Unknown object type at index: %d\n", index);
        break;
    }

    // Shift objects down in the array to fill the gap
    for (int i = index; i < objectManager.count - 1; ++i) {
        objectManager.objects[i] = objectManager.objects[i + 1];
        printf("Shifting object from index %d to %d\n", i + 1, i);
    }

    // Decrement the count of objects after shifting
    objectManager.count--;

    // Clear the last object 
    objectManager.objects[objectManager.count].id = -1;
    printf("Object count after removal: %d\n", objectManager.count);

    // Update selected object if necessary
    if (selected_object == obj) {
        selected_object = NULL;
        printf("Selected object was removed. Clearing selection.\n");
    }
}
void cleanupObjects() {
    for (int i = 0; i < objectManager.count; i++) {
        removeObject(i);
    }
    objectManager.count = 0;
}

void updateObjectInManager(SceneObject* updatedObject) {
    for (int i = 0; i < objectManager.count; i++) {
        if (objectManager.objects[i].id == updatedObject->id) {
            objectManager.objects[i] = *updatedObject;


            printf("Updated object in manager: ID=%d, Index=%d\n", updatedObject->id, i);

            // Update the selected_object pointer if necessary
            if (selected_object && selected_object->id == updatedObject->id) {
                selected_object = &objectManager.objects[i];
            }
            break;
        }
    }
}

void drawObject(const SceneObject* obj, const Matrix4x4 viewMatrix, const Matrix4x4 projMatrix) {
    glUseProgram(shaderProgram);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    Matrix4x4 modelMatrix = translateMatrix(obj->position);
    modelMatrix = matrixMultiply(modelMatrix, rotateMatrix(obj->rotation.x, (Vector3) { 1.0f, 0.0f, 0.0f }));
    modelMatrix = matrixMultiply(modelMatrix, rotateMatrix(obj->rotation.y, (Vector3) { 0.0f, 1.0f, 0.0f }));
    modelMatrix = matrixMultiply(modelMatrix, rotateMatrix(obj->rotation.z, (Vector3) { 0.0f, 0.0f, 1.0f }));
    modelMatrix = matrixMultiply(modelMatrix, scaleMatrix(obj->scale));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix.data[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix.data[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projMatrix.data[0][0]);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
    glUniform4f(colorLoc, obj->color.x, obj->color.y, obj->color.z, obj->color.w);

    if (obj->object.useTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj->object.textureID);
    }

    switch (obj->object.type) {
    case OBJ_CUBE:
        glBindVertexArray(obj->object.data.cube.vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        break;
    case OBJ_SPHERE:
        glBindVertexArray(obj->object.data.sphere.vao);
        glDrawElements(GL_TRIANGLES, obj->object.data.sphere.numIndices, GL_UNSIGNED_INT, 0);
        break;
    case OBJ_PYRAMID:
        glBindVertexArray(obj->object.data.pyramid.vao);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        break;
    case OBJ_CYLINDER:
        glBindVertexArray(obj->object.data.cylinder.vao);
        glDrawElements(GL_TRIANGLES, obj->object.data.cylinder.sectorCount * 12, GL_UNSIGNED_INT, 0);
        break;
    case OBJ_PLANE:
        glBindVertexArray(obj->object.data.plane.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        break;
    case OBJ_MODEL:
        for (unsigned int i = 0; i < obj->object.data.model.meshCount; i++) {
            drawMesh(&obj->object.data.model.meshes[i]);
        }
        break;
    }
    glBindVertexArray(0);
}
