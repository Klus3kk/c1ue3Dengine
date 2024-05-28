#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 vertexColor;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light lights[10];
uniform int lightCount;
uniform sampler2D texture1;
uniform vec3 viewPos;
uniform bool useTexture;
uniform bool useLighting;
uniform bool useColor;
uniform bool noShading;
uniform bool usePBR;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

vec3 calculateLighting(vec3 norm, vec3 viewDir, vec3 albedo, float metallic, float roughness, float ao) {
    vec3 ambient = 0.3 * albedo;
    vec3 lighting = vec3(0.0);

    for (int i = 0; i < lightCount; i++) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * albedo;

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 2.0 / (roughness + 0.0001));
        float kSpecular = (metallic + (1.0 - metallic) * pow(1.0 - max(dot(viewDir, halfwayDir), 0.0), 5.0));
        vec3 specular = spec * lights[i].color * kSpecular;

        lighting += (diffuse + specular) * (1.0 / (1.0 + pow(length(lights[i].position - FragPos), 2.0)));
    }

    return ambient + lighting;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 baseColor = vec3(1.0); // Start with default white color

    if (usePBR) {
        baseColor = texture(albedoMap, TexCoord).rgb;
        norm = normalize(texture(normalMap, TexCoord).rgb * 2.0 - 1.0);
        float metallic = texture(metallicMap, TexCoord).r;
        float roughness = texture(roughnessMap, TexCoord).r;
        float ao = texture(aoMap, TexCoord).r;
        baseColor *= ao; // Apply ambient occlusion directly to base color
    } else if (useTexture) {
        baseColor = texture(texture1, TexCoord).rgb;
    }

    // Apply vertex color if enabled
    if (useColor) {
        baseColor = mix(baseColor, vertexColor.rgb, 0.5);
    }

    // Compute lighting or return the color directly if no shading is required
    if (noShading || !useLighting) {
        FragColor = vec4(baseColor, 0.5);
    } else {
        vec3 lightingResult = calculateLighting(norm, viewDir, baseColor, 0.0, 1.0, 1.0);
        FragColor = vec4(lightingResult, 1.0);
    }
}
