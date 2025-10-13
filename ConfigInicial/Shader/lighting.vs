#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords; // Atributo para las coordenadas de textura

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords; // Se envían las coordenadas de textura al fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(NormalMatrix * aNormal);
    TexCoords = aTexCoords; // Pasa las coordenadas de textura

    gl_Position = projection * view * vec4(FragPos, 1.0);
}