#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
} light;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 texColor = texture(texSampler, fragTexCoord);

    // Luz ambiente base
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.color;

    // Direção e distância até a luz
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);

    // Atenuação forte pela distância
    float attenuation = 1.0 / (distance * distance); // quanto mais longe, mais escuro

    // Luz difusa básica (se quiser)
    float diff = max(dot(normalize(fragNormal), lightDir), 0.0);
    vec3 diffuse = diff * light.color * attenuation * light.intensity;

    // Combina tudo
    vec3 finalLighting = ambient + diffuse;
    vec3 resultColor = fragColor * texColor.rgb * finalLighting;

    outColor = vec4(resultColor, texColor.a);
}
