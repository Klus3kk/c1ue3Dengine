#pragma once
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
typedef struct {
	GLFWwindow* window;
	int width;
	int height;
	char title[100];
} Screen;