#ifndef RENDERING_H
#define RENDERING_H

#include <GLFW/glfw3.h>
#include "Camera.h" 
#include "Vectors.h"
#include "3DObjects.h"

// Function prototypes
void setup();
void render();
double calculateDeltaTime();
void update(double deltaTime);
void handleMouseInput(GLFWwindow* window, Camera* camera);
void end();
void loadResources(float* progress);

// Input callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif 
