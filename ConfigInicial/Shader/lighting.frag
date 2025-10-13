#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords; // Recibe las coordenadas de textura

// Estructura para la luz
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Estructura para el material (AHORA INCLUYE LA TEXTURA)
struct Material {
    sampler2D diffuse; // La textura del modelo
    float shininess;
};

uniform Light light;
uniform Material material; // Se usa la estructura completa
uniform vec3 viewPos;

// Uniforms para la fuente de luz
uniform bool isLightSource;
uniform vec3 lightColor;

void main()
{
    // Si el objeto es el Sol o la Luna, lo pinta de un color sólido y termina.
    if (isLightSource) {
        FragColor = vec4(lightColor, 1.0);
        return;
    }

    // --- Si es un objeto normal, calcula la luz usando su textura ---
    vec3 texColor = texture(material.diffuse, TexCoords).rgb;

    // Ambiente
    vec3 ambient = light.ambient * texColor;

    // Difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * texColor);

    // Especular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec; // El brillo especular es del color de la luz

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}