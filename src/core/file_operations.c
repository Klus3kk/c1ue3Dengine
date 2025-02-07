#include "file_operations.h"
#include "globals.h"
#include "gui.h" 
#include "Camera.h"
#include "ObjectManager.h"
#include "lightshading.h"
#include "SceneObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_operations/tinyfiledialogs.h"
#include "cJSON/cJSON.h"

extern Camera camera;
extern ObjectManager objectManager;
extern Light lights[MAX_LIGHTS];
extern int lightCount;

const char* getMaterialName(PBRMaterial* material) {
    for (int i = 0; i < materialCount; i++) {
        if (&materials[i] == material) {
            return materialNames[i];
        }
    }
    return "";
}

const char* object_type_to_string(ObjectType type) {
    switch (type) {
    case OBJ_CUBE: return "cube";
    case OBJ_SPHERE: return "sphere";
    case OBJ_PYRAMID: return "pyramid";
    case OBJ_CYLINDER: return "cylinder";
    case OBJ_PLANE: return "plane";
    case OBJ_MODEL: return "model";
    default: return "unknown";
    }
}

ObjectType string_to_object_type(const char* type) {
    if (strcmp(type, "cube") == 0) return OBJ_CUBE;
    if (strcmp(type, "sphere") == 0) return OBJ_SPHERE;
    if (strcmp(type, "pyramid") == 0) return OBJ_PYRAMID;
    if (strcmp(type, "cylinder") == 0) return OBJ_CYLINDER;
    if (strcmp(type, "plane") == 0) return OBJ_PLANE;
    if (strcmp(type, "model") == 0) return OBJ_MODEL;
    return OBJ_CUBE;  // Default type
}

void save_project() {
    char const* filterPatterns[1] = { "*.json" };
    char const* savePath = tinyfd_saveFileDialog(
        "Save Project As",
        "project.json",
        1,
        filterPatterns,
        "JSON Files"
    );

    if (!savePath) {
        fprintf(stderr, "Save operation cancelled or failed to get a valid path.\n");
        return;
    }

    cJSON* root = cJSON_CreateObject();

    // Save Objects
    cJSON* objectsArray = cJSON_AddArrayToObject(root, "objects");
    for (int i = 0; i < objectManager.count; i++) {
        SceneObject* obj = &objectManager.objects[i];
        cJSON* jsonObject = cJSON_CreateObject();

        cJSON_AddStringToObject(jsonObject, "type", object_type_to_string(obj->object.type));
        cJSON_AddNumberToObject(jsonObject, "positionX", obj->position.x);
        cJSON_AddNumberToObject(jsonObject, "positionY", obj->position.y);
        cJSON_AddNumberToObject(jsonObject, "positionZ", obj->position.z);
        cJSON_AddNumberToObject(jsonObject, "rotationX", obj->rotation.x);
        cJSON_AddNumberToObject(jsonObject, "rotationY", obj->rotation.y);
        cJSON_AddNumberToObject(jsonObject, "rotationZ", obj->rotation.z);
        cJSON_AddNumberToObject(jsonObject, "scaleX", obj->scale.x);
        cJSON_AddNumberToObject(jsonObject, "scaleY", obj->scale.y);
        cJSON_AddNumberToObject(jsonObject, "scaleZ", obj->scale.z);
        cJSON_AddNumberToObject(jsonObject, "colorR", obj->color.x);
        cJSON_AddNumberToObject(jsonObject, "colorG", obj->color.y);
        cJSON_AddNumberToObject(jsonObject, "colorB", obj->color.z);
        cJSON_AddNumberToObject(jsonObject, "colorA", obj->color.w);

        cJSON_AddNumberToObject(jsonObject, "useTexture", obj->object.useTexture);
        cJSON_AddNumberToObject(jsonObject, "textureID", obj->object.textureID);
        cJSON_AddNumberToObject(jsonObject, "usePBR", obj->object.usePBR);
        cJSON_AddStringToObject(jsonObject, "materialName", getMaterialName(&obj->object.material));

        if (obj->object.type == OBJ_MODEL) {
            cJSON_AddStringToObject(jsonObject, "modelPath", obj->object.data.model.path);
        }

        cJSON_AddItemToArray(objectsArray, jsonObject);
    }

    // Save Lights
    cJSON* lightsArray = cJSON_AddArrayToObject(root, "lights");
    for (int i = 0; i < lightCount; i++) {
        Light* light = &lights[i];
        cJSON* jsonLight = cJSON_CreateObject();

        cJSON_AddNumberToObject(jsonLight, "type", light->type);
        cJSON_AddNumberToObject(jsonLight, "positionX", light->position.x);
        cJSON_AddNumberToObject(jsonLight, "positionY", light->position.y);
        cJSON_AddNumberToObject(jsonLight, "positionZ", light->position.z);
        cJSON_AddNumberToObject(jsonLight, "directionX", light->direction.x);
        cJSON_AddNumberToObject(jsonLight, "directionY", light->direction.y);
        cJSON_AddNumberToObject(jsonLight, "directionZ", light->direction.z);
        cJSON_AddNumberToObject(jsonLight, "colorR", light->color.x);
        cJSON_AddNumberToObject(jsonLight, "colorG", light->color.y);
        cJSON_AddNumberToObject(jsonLight, "colorB", light->color.z);
        cJSON_AddNumberToObject(jsonLight, "intensity", light->intensity);
        cJSON_AddNumberToObject(jsonLight, "constant", light->constant);
        cJSON_AddNumberToObject(jsonLight, "linear", light->linear);
        cJSON_AddNumberToObject(jsonLight, "quadratic", light->quadratic);
        cJSON_AddNumberToObject(jsonLight, "cutOff", light->cutOff);
        cJSON_AddNumberToObject(jsonLight, "outerCutOff", light->outerCutOff);

        cJSON_AddItemToArray(lightsArray, jsonLight);
    }

    // Save Camera
    cJSON* cameraObject = cJSON_AddObjectToObject(root, "camera");
    cJSON_AddNumberToObject(cameraObject, "positionX", camera.Position.x);
    cJSON_AddNumberToObject(cameraObject, "positionY", camera.Position.y);
    cJSON_AddNumberToObject(cameraObject, "positionZ", camera.Position.z);
    cJSON_AddNumberToObject(cameraObject, "frontX", camera.Front.x);
    cJSON_AddNumberToObject(cameraObject, "frontY", camera.Front.y);
    cJSON_AddNumberToObject(cameraObject, "frontZ", camera.Front.z);
    cJSON_AddNumberToObject(cameraObject, "upX", camera.Up.x);
    cJSON_AddNumberToObject(cameraObject, "upY", camera.Up.y);
    cJSON_AddNumberToObject(cameraObject, "upZ", camera.Up.z);
    cJSON_AddNumberToObject(cameraObject, "rightX", camera.Right.x);
    cJSON_AddNumberToObject(cameraObject, "rightY", camera.Right.y);
    cJSON_AddNumberToObject(cameraObject, "rightZ", camera.Right.z);
    cJSON_AddNumberToObject(cameraObject, "worldUpX", camera.WorldUp.x);
    cJSON_AddNumberToObject(cameraObject, "worldUpY", camera.WorldUp.y);
    cJSON_AddNumberToObject(cameraObject, "worldUpZ", camera.WorldUp.z);
    cJSON_AddNumberToObject(cameraObject, "yaw", camera.Yaw);
    cJSON_AddNumberToObject(cameraObject, "pitch", camera.Pitch);
    cJSON_AddNumberToObject(cameraObject, "movementSpeed", camera.MovementSpeed);
    cJSON_AddNumberToObject(cameraObject, "mouseSensitivity", camera.MouseSensitivity);
    cJSON_AddNumberToObject(cameraObject, "zoom", camera.Zoom);
    cJSON_AddBoolToObject(cameraObject, "invertY", camera.invertY);
    cJSON_AddNumberToObject(cameraObject, "mode", camera.mode);

    // Save Background
    cJSON* backgroundObject = cJSON_AddObjectToObject(root, "background");
    cJSON_AddBoolToObject(backgroundObject, "enabled", backgroundEnabled);

    // Save toggles
    cJSON_AddBoolToObject(root, "isRunning", isRunning);
    cJSON_AddBoolToObject(root, "texturesEnabled", texturesEnabled);
    cJSON_AddBoolToObject(root, "colorsEnabled", colorsEnabled);
    cJSON_AddBoolToObject(root, "lightingEnabled", lightingEnabled);
    cJSON_AddBoolToObject(root, "noShading", noShading);
    cJSON_AddBoolToObject(root, "usePBR", usePBR);

    char* jsonString = cJSON_Print(root);
    FILE* file = fopen(savePath, "w");
    if (file) {
        fprintf(file, "%s", jsonString);
        fclose(file);
    }

    cJSON_Delete(root);
    free(jsonString);
}

void load_project() {
    char const* filterPatterns[2] = { "*.json", "*.txt" };
    char const* loadPath = tinyfd_openFileDialog(
        "Open Project",
        "",
        2,
        filterPatterns,
        "Project Files",
        0
    );

    if (!loadPath) {
        fprintf(stderr, "Load operation cancelled or failed to get a valid path.\n");
        return;
    }

    FILE* file = fopen(loadPath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* jsonString = malloc(length + 1);
    if (!jsonString) {
        fprintf(stderr, "Failed to allocate memory for JSON string.\n");
        fclose(file);
        return;
    }

    fread(jsonString, 1, length, file);
    fclose(file);
    jsonString[length] = '\0';

    cJSON* root = cJSON_Parse(jsonString);
    if (!root) {
        fprintf(stderr, "Failed to parse JSON file.\n");
        free(jsonString);
        return;
    }

    // Clear current objects and lights
    cleanupObjects();
    lightCount = 0;

    // Load Objects
    cJSON* objectsArray = cJSON_GetObjectItem(root, "objects");
    if (objectsArray) {
        int arraySize = cJSON_GetArraySize(objectsArray);
        for (int i = 0; i < arraySize; i++) {
            cJSON* jsonObject = cJSON_GetArrayItem(objectsArray, i);

            ObjectType type = string_to_object_type(cJSON_GetObjectItem(jsonObject, "type")->valuestring);
            Vector3 position = {
                cJSON_GetObjectItem(jsonObject, "positionX")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "positionY")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "positionZ")->valuedouble
            };
            Vector3 rotation = {
                cJSON_GetObjectItem(jsonObject, "rotationX")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "rotationY")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "rotationZ")->valuedouble
            };
            Vector3 scale = {
                cJSON_GetObjectItem(jsonObject, "scaleX")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "scaleY")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "scaleZ")->valuedouble
            };
            Vector4 color = {
                cJSON_GetObjectItem(jsonObject, "colorR")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "colorG")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "colorB")->valuedouble,
                cJSON_GetObjectItem(jsonObject, "colorA")->valuedouble
            };
            bool useTexture = cJSON_GetObjectItem(jsonObject, "useTexture")->valueint;
            int textureID = cJSON_GetObjectItem(jsonObject, "textureID")->valueint;
            bool usePBR = cJSON_GetObjectItem(jsonObject, "usePBR")->valueint;
            const char* materialName = cJSON_GetObjectItem(jsonObject, "materialName")->valuestring;

            PBRMaterial* material = getMaterial(materialName);
            if (!material) {
                printf("Warning: Material '%s' not found. Using default material 'peacockOre'.\n", materialName);
                material = getMaterial("peacockOre");
            }

            if (type == OBJ_MODEL) {
                const char* modelPath = cJSON_GetObjectItem(jsonObject, "modelPath")->valuestring;
                Model* model = loadModel(modelPath);
                if (model) {
                    addObject(&camera, type, useTexture, textureID, true, model, *material, usePBR);
                }
                else {
                    printf("Error: Failed to load model from path: %s\n", modelPath);
                }
            }
            else {
                addObject(&camera, type, useTexture, textureID, true, NULL, *material, usePBR);
            }

            SceneObject* newObj = &objectManager.objects[objectManager.count - 1];
            newObj->position = position;
            newObj->rotation = rotation;
            newObj->scale = scale;
            newObj->color = color;
        }
    }

    // Load Lights
    cJSON* lightsArray = cJSON_GetObjectItem(root, "lights");
    if (lightsArray) {
        int arraySize = cJSON_GetArraySize(lightsArray);
        for (int i = 0; i < arraySize; i++) {
            cJSON* jsonLight = cJSON_GetArrayItem(lightsArray, i);

            Light newLight;
            newLight.type = cJSON_GetObjectItem(jsonLight, "type")->valueint;
            newLight.position.x = cJSON_GetObjectItem(jsonLight, "positionX")->valuedouble;
            newLight.position.y = cJSON_GetObjectItem(jsonLight, "positionY")->valuedouble;
            newLight.position.z = cJSON_GetObjectItem(jsonLight, "positionZ")->valuedouble;
            newLight.direction.x = cJSON_GetObjectItem(jsonLight, "directionX")->valuedouble;
            newLight.direction.y = cJSON_GetObjectItem(jsonLight, "directionY")->valuedouble;
            newLight.direction.z = cJSON_GetObjectItem(jsonLight, "directionZ")->valuedouble;
            newLight.color.x = cJSON_GetObjectItem(jsonLight, "colorR")->valuedouble;
            newLight.color.y = cJSON_GetObjectItem(jsonLight, "colorG")->valuedouble;
            newLight.color.z = cJSON_GetObjectItem(jsonLight, "colorB")->valuedouble;
            newLight.intensity = cJSON_GetObjectItem(jsonLight, "intensity")->valuedouble;
            newLight.constant = cJSON_GetObjectItem(jsonLight, "constant")->valuedouble;
            newLight.linear = cJSON_GetObjectItem(jsonLight, "linear")->valuedouble;
            newLight.quadratic = cJSON_GetObjectItem(jsonLight, "quadratic")->valuedouble;
            newLight.cutOff = cJSON_GetObjectItem(jsonLight, "cutOff")->valuedouble;
            newLight.outerCutOff = cJSON_GetObjectItem(jsonLight, "outerCutOff")->valuedouble;

            addLight(newLight);
        }
    }

    // Load Camera
    cJSON* cameraObject = cJSON_GetObjectItem(root, "camera");
    if (cameraObject) {
        camera.Position.x = cJSON_GetObjectItem(cameraObject, "positionX")->valuedouble;
        camera.Position.y = cJSON_GetObjectItem(cameraObject, "positionY")->valuedouble;
        camera.Position.z = cJSON_GetObjectItem(cameraObject, "positionZ")->valuedouble;
        camera.Front.x = cJSON_GetObjectItem(cameraObject, "frontX")->valuedouble;
        camera.Front.y = cJSON_GetObjectItem(cameraObject, "frontY")->valuedouble;
        camera.Front.z = cJSON_GetObjectItem(cameraObject, "frontZ")->valuedouble;
        camera.Up.x = cJSON_GetObjectItem(cameraObject, "upX")->valuedouble;
        camera.Up.y = cJSON_GetObjectItem(cameraObject, "upY")->valuedouble;
        camera.Up.z = cJSON_GetObjectItem(cameraObject, "upZ")->valuedouble;
        camera.Right.x = cJSON_GetObjectItem(cameraObject, "rightX")->valuedouble;
        camera.Right.y = cJSON_GetObjectItem(cameraObject, "rightY")->valuedouble;
        camera.Right.z = cJSON_GetObjectItem(cameraObject, "rightZ")->valuedouble;
        camera.WorldUp.x = cJSON_GetObjectItem(cameraObject, "worldUpX")->valuedouble;
        camera.WorldUp.y = cJSON_GetObjectItem(cameraObject, "worldUpY")->valuedouble;
        camera.WorldUp.z = cJSON_GetObjectItem(cameraObject, "worldUpZ")->valuedouble;
        camera.Yaw = cJSON_GetObjectItem(cameraObject, "yaw")->valuedouble;
        camera.Pitch = cJSON_GetObjectItem(cameraObject, "pitch")->valuedouble;
        camera.MovementSpeed = cJSON_GetObjectItem(cameraObject, "movementSpeed")->valuedouble;
        camera.MouseSensitivity = cJSON_GetObjectItem(cameraObject, "mouseSensitivity")->valuedouble;
        camera.Zoom = cJSON_GetObjectItem(cameraObject, "zoom")->valuedouble;
        camera.invertY = cJSON_GetObjectItem(cameraObject, "invertY")->valueint;
    }

    cJSON_Delete(root);
    free(jsonString);
}


void new_project() {
    cleanupObjects();
    lightCount = 0;
    selected_object = NULL; // Reset the selected object

    // Optionally reset other state variables as needed
    camera.Position = (Vector3){ 0.0f, 0.0f, 3.0f };
    camera.Front = (Vector3){ 0.0f, 0.0f, -1.0f };
    camera.Up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.Right = (Vector3){ 1.0f, 0.0f, 0.0f };
    camera.WorldUp = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.Yaw = -90.0f;
    camera.Pitch = 0.0f;
    camera.MovementSpeed = 5.0f;
    camera.MouseSensitivity = 0.5f;
    camera.Zoom = 45.0f;
    camera.invertY = false;
    camera.mode = CAMERA_MODE_ORBIT;

    backgroundEnabled = true;
    isRunning = false; // Start paused
    texturesEnabled = true;
    colorsEnabled = true;
    lightingEnabled = true;
    noShading = false;
    usePBR = true;

    reset_gui(); // Reset GUI to initial state
}

