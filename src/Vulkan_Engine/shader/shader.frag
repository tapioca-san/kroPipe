#version 450

layout(binding = 1) uniform sampler2D texSampler;

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
};

layout(binding = 2) uniform LightUBO {
    Light light;
};

// entradas do vertex shader com `location`
layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Color;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 baseColor = texture(texSampler, TexCoords).rgb * Color;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.color;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (distance * distance);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color * attenuation * light.intensity;

    vec3 result = (ambient + diffuse) * baseColor;
    outColor = vec4(result, 1.0);
}