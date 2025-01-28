#include <glad/glad.h>

unsigned int loadShader(const char* vertexPath, const char* fragmentPath);
void checkCompileErrors(unsigned int shader, const char* type);
char* readFile(const char* filePath);

