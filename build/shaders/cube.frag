#version 330 core

in vec3 normalVec;
in vec3 fragPos;
  
out vec4 FragColor;

uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    // Ambient  
    vec3 ambientVec = material.ambient * light.ambient;

    // Diffuse
    vec3 normal = normalize(normalVec);
    vec3 rayDir = normalize(light.position - fragPos); // A - B means B -> A 
    float diffuse = max(dot(normal, rayDir), 0.0);
    vec3 diffuseVec =  diffuse * material.diffuse * light.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-rayDir, normal); // We want from A -> B
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularVec = specular * material.specular * light.specular;


    vec3 result = specularVec + ambientVec + diffuseVec;
    FragColor = vec4(result, 1.0);
}