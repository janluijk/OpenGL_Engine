#version 330 core

// STRUCTS
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

/* CODE */
out vec4 FragColor;

in vec3 fNormalVec;
in vec4 fWorldPos;
in vec2 fTexCoords;

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// Main
void main()
{
    // Ambient  
    vec3 ambientVec     = light.ambient * texture(material.diffuse, fTexCoords).rgb;

    // Diffuse
    vec3 rayDir         = normalize(light.position - fWorldPos.xyz); // A - B means B -> A 
    float diffuse       = max(dot(fNormalVec, rayDir), 0.0);
    vec3 diffuseVec     = diffuse * vec3(texture(material.diffuse, fTexCoords)) * light.diffuse;

    // specular 
    vec3 viewDir        = normalize(viewPos - fWorldPos.xyz);
    vec3 reflectDir     = reflect(-rayDir, fNormalVec); // We want from A -> B
    float specular      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularVec    = specular * vec3(texture(material.specular, fTexCoords)) * light.specular;

    vec3 result         = specularVec + ambientVec + diffuseVec;
    FragColor           = vec4(result, 1.0);
}