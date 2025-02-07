#include "shaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
// Function to read the content of a shader source file
char* readFile(const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open the file: %s\n", filePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* data = (char*)malloc(length + 1);
    if (!data) {
        fclose(file);
        fprintf(stderr, "Failed to allocate memory\n");
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';

    fclose(file);
    return data;
}

// Function to check and log shader and program compilation/linking errors
bool checkCompileErrors(unsigned int shader, const char* type) {
    int success;
    char infoLog[512];
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
            return false;
        }
    }
    return true;
}

// Function to load and compile shaders, and link them into a program
unsigned int loadShader(const char* vertexPath, const char* fragmentPath) {
    char* vShaderCode = readFile(vertexPath);
    char* fShaderCode = readFile(fragmentPath);
    if (!vShaderCode || !fShaderCode) {
        if (vShaderCode) free(vShaderCode);
        if (fShaderCode) free(fShaderCode);
        return 0;
    }

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar* const*)&vShaderCode, NULL);
    glCompileShader(vertex);
    if (!checkCompileErrors(vertex, "VERTEX")) {
        free(vShaderCode);
        free(fShaderCode);
        glDeleteShader(vertex);
        return 0;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar* const*)&fShaderCode, NULL);
    glCompileShader(fragment);
    if (!checkCompileErrors(fragment, "FRAGMENT")) {
        free(vShaderCode);
        free(fShaderCode);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return 0;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    if (!checkCompileErrors(shaderProgram, "PROGRAM")) {
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        free(vShaderCode);
        free(fShaderCode);
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    free(vShaderCode);
    free(fShaderCode);

    return shaderProgram;
}
