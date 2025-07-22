#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

// Entrada apenas com bones e posição
layout(location = 0) in vec3 inPosition;
layout(location = 7) in vec4 boneIds;
layout(location = 8) in vec4 boneWeights;

// Saída de fragmento (visualizar peso dos ossos como cor)
layout(location = 0) out vec4 BoneWeightColor;

void main() {
    vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * worldPosition;

    // Envia pesos para o fragment shader (como cor visual)
    BoneWeightColor = vec4(boneWeights.rgb, 1.0);
}
