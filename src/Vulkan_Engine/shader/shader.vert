#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 vertexColors;
layout(location = 4) in vec4 Normal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragPos;    // ← posição do fragmento no mundo
layout(location = 3) out vec3 fragNormal; // ← normal no espaço mundial

void main() {
    vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * worldPosition;

    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragPos = worldPosition.xyz;
    fragNormal = mat3(transpose(inverse(ubo.model))) * Normal.xyz; 
}
