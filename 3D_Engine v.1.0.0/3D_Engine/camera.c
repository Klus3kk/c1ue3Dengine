#include "Camera.h"
#include "globals.h" // Include this to access global variables
#include "ObjectManager.h" // Include this to access addObject function
#include "Vectors.h"
#include "materials.h" // Include this to access PBRMaterial
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>

// Define the global camera variable
Camera camera;
#define M_PI 3.14159265358979323846
#define FORWARD  1
#define BACKWARD 2
#define LEFT     3
#define RIGHT    4
#define SPACE    5
#define SHIFT    6

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

extern SelectedType selectedType; // Ensure the selectedType is accessible here

static bool isDragging = false;
static bool isMoving = false;
static bool isPanning = false;
static double lastX, lastY;

void updateCameraVectors(Camera* camera) {
    Vector3 front;
    front.x = cos(M_PI / 180.0 * camera->Yaw) * cos(M_PI / 180.0 * camera->Pitch);
    front.y = sin(M_PI / 180.0 * camera->Pitch);
    front.z = sin(M_PI / 180.0 * camera->Yaw) * cos(M_PI / 180.0 * camera->Pitch);
    camera->Front = vector_normalize(front);
    camera->Right = vector_normalize(vector_cross(camera->Front, camera->WorldUp));
    camera->Up = vector_normalize(vector_cross(camera->Right, camera->Front));
    camera->view = getViewMatrix(camera);  // Update view matrix
}


void processMousePan(Camera* camera, float xoffset, float yoffset) {
    // Adjust the sensitivity for panning
    float sensitivity = 0.01f;  // Adjust this value as needed
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Calculate the right and up vectors based on the current camera's direction
    Vector3 right = vector_normalize(vector_cross(camera->Front, camera->WorldUp));
    Vector3 up = vector_normalize(camera->Up);

    // Update the camera's position: Move right for xoffset and up/down for yoffset
    camera->Position = vector_add(camera->Position, vector_scale(right, xoffset));
    camera->Position = vector_add(camera->Position, vector_scale(up, yoffset));
    camera->view = getViewMatrix(camera);  // Update the view matrix
}





void initCamera(Camera* camera) {
    camera->Position = vector(0.0f, 0.0f, 3.0f);
    camera->WorldUp = vector(0.0f, 1.0f, 0.0f);
    camera->Yaw = -90.0f;
    camera->Pitch = 0.0f;
    camera->MovementSpeed = 5.0f;
    camera->MouseSensitivity = 0.5f;
    camera->Zoom = 45.0f;
    camera->invertY = false;
    camera->mode = CAMERA_MODE_FREE; // Set default camera mode
    updateCameraVectors(camera);
    glfwSetMouseButtonCallback(screen.window, mouse_button_callback);
    glfwSetCursorPosCallback(screen.window, cursor_position_callback);
    glfwSetScrollCallback(screen.window, scroll_callback);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && (mods & GLFW_MOD_ALT)) {
        isPanning = true;
        glfwGetCursorPos(window, &lastX, &lastY);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        isPanning = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        isDragging = true;
        glfwGetCursorPos(window, &lastX, &lastY);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        isDragging = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging || isPanning) {
        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        if (isPanning) {
            processMousePan(&camera, (float)xoffset, (float)yoffset);
        }
        else if (isDragging) {
            processMouseMovement(&camera, (float)xoffset, (float)yoffset, true);
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    processMouseScroll(&camera, (float)yoffset);
}

void processKeyboard(Camera* camera, int direction, float deltaTime) {
    float velocity = camera->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        camera->Position = vector_add(camera->Position, vector_scale(camera->Front, velocity));
    if (direction == BACKWARD)
        camera->Position = vector_sub(camera->Position, vector_scale(camera->Front, velocity));
    if (direction == LEFT)
        camera->Position = vector_sub(camera->Position, vector_scale(camera->Right, velocity));
    if (direction == RIGHT)
        camera->Position = vector_add(camera->Position, vector_scale(camera->Right, velocity));
    if (direction == SPACE)
        camera->Position = vector_add(camera->Position, vector_scale(camera->Up, velocity));
    if (direction == SHIFT)
        camera->Position = vector_sub(camera->Position, vector_scale(camera->Up, velocity));
    camera->view = getViewMatrix(camera);  // Update view matrix
}

void processMouseMovement(Camera* camera, float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= camera->MouseSensitivity;
    yoffset *= camera->MouseSensitivity;

    camera->Yaw += xoffset;
    camera->Pitch += yoffset;

    if (constrainPitch) {
        if (camera->Pitch > 89.0f)
            camera->Pitch = 89.0f;
        if (camera->Pitch < -89.0f)
            camera->Pitch = -89.0f;
    }

    updateCameraVectors(camera);
}


void processMouseScroll(Camera* camera, float yoffset) {
    camera->Zoom -= yoffset;
    if (camera->Zoom < 1.0f)
        camera->Zoom = 1.0f;
    if (camera->Zoom > 45.0f)
        camera->Zoom = 45.0f;
}

Matrix4x4 getViewMatrix(Camera* camera) {
    return lookAt(camera->Position, vector_add(camera->Position, camera->Front), camera->Up);
}

Matrix4x4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
    return perspective(fov, aspectRatio, nearPlane, farPlane);
}

Matrix4x4 translateMatrix(Vector3 position) {
    Matrix4x4 mat = { 0 };
    for (int i = 0; i < 4; i++) {
        mat.data[i][i] = 1.0f;
    }
    mat.data[3][0] = position.x;
    mat.data[3][1] = position.y;
    mat.data[3][2] = position.z;
    return mat;
}

Matrix4x4 rotateMatrix(float angle, Vector3 axis) {
    Matrix4x4 mat = { 0 };
    float rad = angle * M_PI / 180.0f;
    float c = cos(rad);
    float s = sin(rad);
    Vector3 norm = vector_normalize(axis);
    mat.data[0][0] = c + norm.x * norm.x * (1 - c);
    mat.data[0][1] = norm.x * norm.y * (1 - c) - norm.z * s;
    mat.data[0][2] = norm.x * norm.z * (1 - c) + norm.y * s;
    mat.data[1][0] = norm.y * norm.x * (1 - c) + norm.z * s;
    mat.data[1][1] = c + norm.y * norm.y * (1 - c);
    mat.data[1][2] = norm.y * norm.z * (1 - c) - norm.x * s;
    mat.data[2][0] = norm.z * norm.x * (1 - c) - norm.y * s;
    mat.data[2][1] = norm.z * norm.y * (1 - c) + norm.x * s;
    mat.data[2][2] = c + norm.z * norm.z * (1 - c);
    mat.data[3][3] = 1.0f;
    return mat;
}

Matrix4x4 scaleMatrix(Vector3 scale) {
    Matrix4x4 mat = { 0 };
    mat.data[0][0] = scale.x;
    mat.data[1][1] = scale.y;
    mat.data[2][2] = scale.z;
    mat.data[3][3] = 1.0f;
    return mat;
}

Matrix4x4 matrixMultiply(Matrix4x4 a, Matrix4x4 b) {
    Matrix4x4 result = { 0 };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return result;
}

Matrix4x4 lookAt(Vector3 eye, Vector3 center, Vector3 up) {
    Vector3 f = vector_normalize(vector_sub(center, eye));
    Vector3 s = vector_normalize(vector_cross(f, up));
    Vector3 u = vector_cross(s, f);

    Matrix4x4 result = { 0 };
    result.data[0][0] = s.x;
    result.data[1][0] = s.y;
    result.data[2][0] = s.z;
    result.data[0][1] = u.x;
    result.data[1][1] = u.y;
    result.data[2][1] = u.z;
    result.data[0][2] = -f.x;
    result.data[1][2] = -f.y;
    result.data[2][2] = -f.z;
    result.data[3][0] = -vector_dot(s, eye);
    result.data[3][1] = -vector_dot(u, eye);
    result.data[3][2] = vector_dot(f, eye);
    result.data[3][3] = 1.0f;
    return result;
}


Matrix4x4 perspective(float fov, float aspect, float znear, float zfar) {
    Matrix4x4 m = { 0 };
    float tanHalfFovy = tan(fov / 2.0f);
    m.data[0][0] = 1.0f / (aspect * tanHalfFovy);
    m.data[1][1] = 1.0f / tanHalfFovy;
    m.data[2][2] = -(zfar + znear) / (zfar - znear);
    m.data[2][3] = -1.0f;
    m.data[3][2] = -(2.0f * zfar * znear) / (zfar - znear);
    return m;
}

Matrix4x4 identityMatrix() {
    Matrix4x4 result = { .data = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    } };
    return result;
}
