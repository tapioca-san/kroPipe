#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 inVertexColor;
layout(location = 4) in vec4 inNormal;
layout(location = 5) in vec3 inTangent;
layout(location = 6) in vec3 inBitangent;
layout(location = 7) in vec4 boneIds;
layout(location = 8) in vec4 boneWeights;


// saídas para o fragment shader (com location!)
layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec2 TexCoords;
layout(location = 3) out vec3 Color;

void main() {
    vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * worldPosition;

    FragPos   = worldPosition.xyz;
    Normal    = mat3(transpose(inverse(ubo.model))) * inNormal.xyz;
    TexCoords = inTexCoord;
    Color     = inColor;
}