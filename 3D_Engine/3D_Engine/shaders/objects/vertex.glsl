#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;  
out vec2 TexCoord;  
out vec3 Normal;   
out vec4 vertexColor;  

uniform mat4 model;       
uniform mat4 view;        
uniform mat4 projection;  
uniform vec4 inputColor;  

void main() {
    vec4 worldPosition = model * vec4(aPos, 1.0);
    FragPos = vec3(worldPosition);  
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoord = aTexCoord;
    vertexColor = inputColor;  
    gl_Position = projection * view * worldPosition;  
}
