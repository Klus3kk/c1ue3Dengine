#ifndef CAMERA_H
#define CAMERA_H

#include "Vectors.h"
#include <stdbool.h>
#include "types.h"
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
typedef enum {
    CAMERA_MODE_FPS,
    CAMERA_MODE_FREE,
    CAMERA_MODE_ORBIT
} CameraMode;

typedef struct {
    Vector3 Position;
    Vector3 Front;
    Vector3 Up;
    Vector3 Right;
    Vector3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool invertY;
    Matrix4x4 view;
    CameraMode mode;
} Camera;

// Define the global camera variable
extern Camera camera;
#define M_PI 3.14159265358979323846
#define FORWARD  1
#define BACKWARD 2
#define LEFT     3
#define RIGHT    4
#define SPACE    5
#define SHIFT    6

extern SelectedType selectedType; 


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void initCamera(Camera* camera);
void updateCameraVectors(Camera* camera);
void processKeyboard(Camera* camera, int direction, float deltaTime);
void processMouseMovement(Camera* camera, float xoffset, float yoffset, bool constrainPitch);
void processMouseScroll(Camera* camera, float yoffset);
void processMousePan(Camera* camera, float xoffset, float yoffset);
Matrix4x4 getViewMatrix(Camera* camera);
Matrix4x4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
Matrix4x4 translateMatrix(Vector3 position);
Matrix4x4 matrixMultiply(Matrix4x4 a, Matrix4x4 b);
Matrix4x4 lookAt(Vector3 eye, Vector3 center, Vector3 up);
Matrix4x4 perspective(float fov, float aspect, float znear, float zfar);
Matrix4x4 rotateMatrix(float angle, Vector3 axis);
Matrix4x4 scaleMatrix(Vector3 scale);
Matrix4x4 identityMatrix();
#endif 
