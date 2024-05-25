#include "textures.h"
#include "SOIL2/SOIL2.h"
#include <stdio.h>

const char* textureNames[] = {
    "blue",
    "bricks",
    "float",
    "img_mars",
    "leather",
    "rubber",
    "test_rect",
};
int textureCount = sizeof(textureNames) / sizeof(textureNames[0]); // No 'const' here
GLuint textures[MAX_TEXTURES];
GLuint getTexture(const char* name) {
    for (int i = 0; i < textureCount; i++) {
        if (strcmp(textureNames[i], name) == 0) {
            return textures[i];
        }
    }
    fprintf(stderr, "Texture %s not found.\n", name);
    return 0;
}


GLuint loadTexture(const char* filename) {
    GLuint textureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if (textureID == 0) {
        fprintf(stderr, "Failed to load texture file %s: %s\n", filename, SOIL_last_result());
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    fprintf(stderr, "Loaded texture %s, ID %u\n", filename, textureID);
    return textureID;
}

void loadAllTextures() {
    const char* textureFiles[] = {
        "resources/textures/objects/blue.jpg",
        "resources/textures/objects/bricks.jpg",
        "resources/textures/objects/float.jpg",
        "resources/textures/objects/img_mars.jpg",
        "resources/textures/objects/leather.jpg",
        "resources/textures/objects/rubber.jpg",
        "resources/textures/objects/test_rect.png",
    };
    int numTextures = sizeof(textureFiles) / sizeof(textureFiles[0]);
    for (int i = 0; i < numTextures; i++) {
        if (i < MAX_TEXTURES) {
            textures[i] = loadTexture(textureFiles[i]);
            if (textures[i] == 0) {
                fprintf(stderr, "Failed to load texture: %s\n", textureFiles[i]);
            }
        }
        else {
            fprintf(stderr, "Exceeded maximum texture limit of %d\n", MAX_TEXTURES);
            break;
        }
    }
}

