#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "loading.h"
#include "globals.h"
#include "Vectors.h"

void initLoadingScreen(GLFWwindow* window) {
    glfwMakeContextCurrent(window);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void endLoadingScreen(GLFWwindow* window) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
}
