#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <stdbool.h>
unsigned int loadShader(const char* vertexPath, const char* fragmentPath);
bool checkCompileErrors(unsigned int shader, const char* type);
char* readFile(const char* filePath);

