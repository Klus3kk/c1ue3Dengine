#include "lightshading.h"
#include <glad/glad.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846
#define DEG_TO_RAD(degrees) ((degrees) * (PI / 180.0))
extern unsigned int shaderProgram;
Light lights[MAX_LIGHTS];
int lightCount = 0;

float clamp(float x, float lower, float upper) {
    return fmax(lower, fmin(x, upper));
}

void updateShaderLights() {
    char uniformBuffer[128];
    GLint lightCountLoc = glGetUniformLocation(shaderProgram, "lightCount");
    glUniform1i(lightCountLoc, lightCount);

    for (int i = 0; i < lightCount; i++) {
        sprintf(uniformBuffer, "lights[%d].position", i);
        glUniform3fv(glGetUniformLocation(shaderProgram, uniformBuffer), 1, (const GLfloat*)&lights[i].position);

        sprintf(uniformBuffer, "lights[%d].color", i);
        glUniform3fv(glGetUniformLocation(shaderProgram, uniformBuffer), 1, (const GLfloat*)&lights[i].color);

        sprintf(uniformBuffer, "lights[%d].intensity", i);
        glUniform1f(glGetUniformLocation(shaderProgram, uniformBuffer), lights[i].intensity);

        if (lights[i].type == LIGHT_DIRECTIONAL) {
            sprintf(uniformBuffer, "lights[%d].direction", i);
            glUniform3fv(glGetUniformLocation(shaderProgram, uniformBuffer), 1, (const GLfloat*)&lights[i].direction);
        }
        else if (lights[i].type == LIGHT_SPOT) {
            sprintf(uniformBuffer, "lights[%d].cutOff", i);
            glUniform1f(glGetUniformLocation(shaderProgram, uniformBuffer), lights[i].cutOff);
            sprintf(uniformBuffer, "lights[%d].outerCutOff", i);
            glUniform1f(glGetUniformLocation(shaderProgram, uniformBuffer), lights[i].outerCutOff);
        }
    }
}

void initLightingSystem() {
    // Initialize the light count to zero
    lightCount = 0;
}


void createLight(Vector3 position, Vector3 direction, Vector3 color, float intensity, LightType type) {
    if (lightCount >= MAX_LIGHTS) {
        printf("Failed to create light: Maximum number of lights reached.\n");
        return;
    }

    Light newLight;
    newLight.type = type;
    newLight.position = position;
    newLight.direction = vector_normalize(direction);
    newLight.color = color;
    newLight.intensity = intensity;

    if (type == LIGHT_POINT) {
        newLight.constant = 1.0f;
        newLight.linear = 0.09f;
        newLight.quadratic = 0.032f;
    }
    else if (type == LIGHT_SPOT) {
        newLight.cutOff = cos(DEG_TO_RAD(12.5f));
        newLight.outerCutOff = cos(DEG_TO_RAD(15.0f));
    }

    lights[lightCount++] = newLight;
    printf("Light created at [%f, %f, %f] with intensity %f\n", position.x, position.y, position.z, intensity);
}


void addLight(Light newLight) {
    if (lightCount < MAX_LIGHTS) {
        lights[lightCount++] = newLight;
    }
}

void updateLight(int index, Light updatedLight) {
    if (index >= 0 && index < lightCount) {
        lights[index] = updatedLight;
    }
}

void removeLight(int index) {
    if (index >= 0 && index < lightCount) {
        for (int i = index; i < lightCount - 1; i++) {
            lights[i] = lights[i + 1];
        }
        lightCount--;
    }
}

Vector3 calculateLighting(Vector3 normal, Vector3 fragPos, Vector3 viewDir) {
    Vector3 ambientLightIntensity = vector(0.1f, 0.1f, 0.1f);
    Vector3 result = vector(0.0f, 0.0f, 0.0f);
    normal = vector_normalize(normal);
    viewDir = vector_normalize(viewDir);

    for (int i = 0; i < lightCount; i++) {
        Light light = lights[i];
        Vector3 lightDir;
        float attenuation;
        Vector3 diffuse;
        Vector3 specular;

        switch (light.type) {
        case LIGHT_POINT:
            lightDir = vector_sub(light.position, fragPos);
            if (vector_length(lightDir) != 0) {
                lightDir = vector_normalize(lightDir);
                float distance = vector_length(lightDir);
                attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
                // Proceed with diffuse and specular calculations
            }
            else {
                attenuation = 0; // No light contribution if distance is zero
            }
            break;
        case LIGHT_DIRECTIONAL:
            lightDir = light.direction; // Assume direction is already normalized in light setup
            attenuation = 1.0; // No attenuation for directional lights
            break;
        case LIGHT_SPOT:
            lightDir = vector_sub(light.position, fragPos);
            if (vector_length(lightDir) != 0) {
                lightDir = vector_normalize(lightDir);
                float theta = vector_dot(lightDir, vector_negate(light.direction));
                float epsilon = light.cutOff - light.outerCutOff;
                float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
                float distance = vector_length(lightDir);
                attenuation = intensity / (light.constant + light.linear * distance + light.quadratic * distance * distance);
                // Proceed with diffuse and specular calculations
            }
            else {
                attenuation = 0; // No light contribution if distance is zero
            }
            break;
        }

        // Calculate diffuse component
        float diff = fmax(vector_dot(normal, lightDir), 0.0);
        diffuse = vector_scale(vector_scale(light.color, diff), attenuation);

        // Calculate specular component
        Vector3 reflectDir = reflect(vector_negate(lightDir), normal);
        float spec = pow(fmax(vector_dot(viewDir, reflectDir), 0.0), 32); // Shininess coefficient
        specular = vector_scale(vector_scale(light.color, spec), attenuation);

        // Accumulate contributions
        result = vector_add(result, vector_add(diffuse, specular));
    }

    // Add ambient light once
    result = vector_add(result, vector_scale(ambientLightIntensity, lightCount));

    return result;
}