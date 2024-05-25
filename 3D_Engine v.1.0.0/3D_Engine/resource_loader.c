#include "resource_loader.h"
#include "textures.h"
#include "materials.h"
#include <stdio.h>

void load_material() {
    // Function to load materials
    // This can open a file dialog to select a material file
    printf("Loading material...\n");
    loadPBRTextures();  // Example to load predefined materials
}

void load_texture() {
    // Function to load textures
    // This can open a file dialog to select a texture file
    printf("Loading texture...\n");
    loadAllTextures();  // Example to load predefined textures
}