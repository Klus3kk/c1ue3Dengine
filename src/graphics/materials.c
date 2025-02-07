#include "materials.h"
#include "textures.h"  
#include <stdio.h>
#include <string.h>

PBRMaterial materials[MAX_MATERIALS];
const char* materialNames[MAX_MATERIALS] = { 
    "peacockOre",
    "rockyAsphalt",
    "chunkyRockface",
    "stainlessSteel", };
int materialCount = 0;

// Load textures and create a PBR material
PBRMaterial loadPBRMaterial(const char* albedo, const char* normal, const char* metallic, const char* roughness, const char* ao) {
    PBRMaterial material;

    material.albedoMap = loadTexture(albedo);
    material.normalMap = loadTexture(normal);
    material.metallicMap = loadTexture(metallic);
    material.roughnessMap = loadTexture(roughness);
    material.aoMap = loadTexture(ao);

    if (material.albedoMap == 0 || material.normalMap == 0 || material.metallicMap == 0 ||
        material.roughnessMap == 0 || material.aoMap == 0) {
        fprintf(stderr, "Failed to load one or more textures for PBR material\n");
    }
    else {
        printf("PBR Material loaded successfully.\n");
    }

    return material;
}

void bindPBRMaterial(PBRMaterial material) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.albedoMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, material.normalMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, material.metallicMap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, material.roughnessMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, material.aoMap);
}

void cleanupPBRMaterial(PBRMaterial* material) {
    glDeleteTextures(1, &material->albedoMap);
    glDeleteTextures(1, &material->normalMap);
    glDeleteTextures(1, &material->metallicMap);
    glDeleteTextures(1, &material->roughnessMap);
    glDeleteTextures(1, &material->aoMap);
    printf("PBR Material resources cleaned up.\n");
}

void loadPBRTextures() {
    // Load each material separately
    PBRMaterial peacockOre = loadPBRMaterial(
        "resources/materials/peacock-ore-unity/peacock-ore_albedo.png",
        "resources/materials/peacock-ore-unity/peacock-ore_normal-ogl.png",
        "resources/materials/peacock-ore-unity/peacock-ore_metallic.psd",
        "resources/materials/peacock-ore-unity/peacock-ore_height.png",
        "resources/materials/peacock-ore-unity/peacock-ore_ao.png"
    );
    addMaterial("peacockOre", peacockOre);

    PBRMaterial rockyAsphalt = loadPBRMaterial(
        "resources/materials/rocky-asphalt1-unity/rocky_asphalt1_albedo.png",
        "resources/materials/rocky-asphalt1-unity/rocky_asphalt1_Normal-ogl.png",
        "resources/materials/rocky-asphalt1-unity/rocky_asphalt1_Metallic.psd",
        "resources/materials/rocky-asphalt1-unity/rocky_asphalt1_Height.png",  // Note: Height as roughness is a placeholder
        "resources/materials/rocky-asphalt1-unity/rocky_asphalt1_ao.png"
    );
    addMaterial("rockyAsphalt", rockyAsphalt);

    PBRMaterial chunkyRockface = loadPBRMaterial(
        "resources/materials/stylized-chunky-rockface-unity/stylized-chunky-rockface_albedo.png",
        "resources/materials/stylized-chunky-rockface-unity/stylized-chunky-rockface_normal-ogl.png",
        "resources/materials/stylized-chunky-rockface-unity/stylized-chunky-rockface_metallic.psd",
        "resources/materials/stylized-chunky-rockface-unity/stylized-chunky-rockface_height.png", 
        "resources/materials/stylized-chunky-rockface-unity/stylized-chunky-rockface_ao.png"
    );
    addMaterial("chunkyRockface", chunkyRockface);

    PBRMaterial stainlessSteel = loadPBRMaterial(
        "resources/materials/used-stainless-steel2-unity/used-stainless-steel2_albedo.png",
        "resources/materials/used-stainless-steel2-unity/used-stainless-steel2_normal-ogl.png",
        "resources/materials/used-stainless-steel2-unity/used-stainless-steel2_metallic.psd",
        "resources/materials/used-stainless-steel2-unity/used-stainless-steel2_height.png",  
        "resources/materials/used-stainless-steel2-unity/used-stainless-steel2_ao.png"
    );
    addMaterial("stainlessSteel", stainlessSteel);
}

void addMaterial(const char* name, PBRMaterial material) {
    if (materialCount >= MAX_MATERIALS) {
        fprintf(stderr, "Max materials limit reached.\n");
        return;
    }
    materialNames[materialCount] = strdup(name);  
    materials[materialCount] = material;
    materialCount++;
    printf("Material %s added successfully.\n", name);
}


PBRMaterial* getMaterial(const char* name) {
    for (int i = 0; i < materialCount; i++) {
        if (strcmp(materialNames[i], name) == 0) {
            return &materials[i];
        }
    }
    fprintf(stderr, "Material %s not found. Using default material 'peacockOre'.\n", name);

    for (int i = 0; i < materialCount; i++) {
        if (strcmp(materialNames[i], "peacockOre") == 0) {
            return &materials[i];
        }
    }
    return NULL; 
}

