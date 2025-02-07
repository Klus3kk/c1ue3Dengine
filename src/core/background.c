#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#include "textures.h"
#include "Camera.h"
#include "background.h"
#include "SOIL2/SOIL2.h"
#include <stdio.h>
GLuint skyboxVAO, skyboxVBO, skyboxShader, skyboxTexture;
extern float skyboxVertices[108];
// Define the background names
const char* backgroundNames[] = {
    "Background 1",
    "Background 2",
    "Background 3",
    "Background 4",
    "Background 5",
    "Background 6",
    "Background 7",
    "Background 8"
};

// Define the number of backgrounds
const int backgroundCount = sizeof(backgroundNames) / sizeof(backgroundNames[0]);
GLuint loadCubemap(const char* faceFiles[6]) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, channels;
    for (int i = 0; i < 6; i++) {
        unsigned char* data = SOIL_load_image(faceFiles[i], &width, &height, &channels, SOIL_LOAD_RGB);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            SOIL_free_image_data(data);
        }
        else {
            fprintf(stderr, "Cubemap texture failed to load at path: %s\n", faceFiles[i]);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

float skyboxVertices[] = {
    // Vertices for a cube
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

void initSkybox(int backgroundIndex) {
    if (backgroundIndex < 1 || backgroundIndex > backgroundCount) {
        fprintf(stderr, "Background index out of range. Please choose from 1 to %d.\n", backgroundCount);
        return;
    }

    // Generate and bind the VAO and VBO
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);

    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    // Set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Format file paths dynamically based on the input index
    const char* directions[6] = { "right", "left", "top", "bottom", "front", "back" };
    char buffer[6][1024];  // Allocate buffer for filenames
    const char* faces[6];

    for (int i = 0; i < 6; i++) {
        snprintf(buffer[i], sizeof(buffer[i]), "resources/textures/skybox/background%d/%s.png", backgroundIndex, directions[i]);
        faces[i] = buffer[i];
    }

    // Load textures and shaders
    skyboxTexture = loadCubemap(faces);
    if (skyboxTexture == 0) {
        fprintf(stderr, "Failed to load skybox textures for background %d\n", backgroundIndex);
        return;
    }

    skyboxShader = loadShader("shaders/skybox/skyboxVertex.glsl", "shaders/skybox/skyboxFragment.glsl");
    if (skyboxShader == 0) {
        fprintf(stderr, "Failed to load skybox shader\n");
        return;
    }
}

void drawSkybox(const Camera* camera, const Matrix4x4* projMatrix) {
    glDepthMask(GL_FALSE); // Disable depth write
    glUseProgram(skyboxShader);

    // Create a view matrix for the skybox (remove translation)
    Matrix4x4 viewMatrixSkybox = getViewMatrix(camera);
    viewMatrixSkybox.data[3][0] = 0;
    viewMatrixSkybox.data[3][1] = 0;
    viewMatrixSkybox.data[3][2] = 0;

    // Set the uniform for the view and projection matrices
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, &viewMatrixSkybox.data[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, &projMatrix->data[0][0]);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(0);
    glDepthMask(GL_TRUE); // Re-enable depth write
}