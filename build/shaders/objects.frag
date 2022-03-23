#version 400 core
out vec4 FragColor;


// STRUCTS
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec4 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec4 color;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
};

#define NR_POINT_LIGHTS 4

in vec3 fNormalVec;
in vec3 fWorldPos;
in vec2 fTexCoords;

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform DirectionalLight directionalLight;

vec3 calcPointLight(PointLight light, vec3 fragPos, vec3 fNormalVec, vec3 viewDir);

// Main
void main()
{
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fWorldPos);
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++) 
    {
        result += calcPointLight(pointLights[i], fWorldPos, fNormalVec, viewDir);
    }
    
    FragColor           = vec4(result, 1.0);
}


vec3 calcPointLight(PointLight light, vec3 fragPos, vec3 fNormalVec, vec3 viewDir) 
{
    // Distance light and cube
    float lightDistance = length(light.position - fWorldPos);
    float attenuation;
    if(light.quadratic == 0.03) {
        attenuation = 1.0 / (light.quadratic * lightDistance * lightDistance);
    }
    


    // Diffuse
    vec3 rayDir         = normalize(light.position - fragPos); // A - B means B -> A 
    float diffuse       = max(dot(fNormalVec, rayDir), 0.0);
    
    // Specular 
    vec3 reflectDir     = reflect(-rayDir, fNormalVec); // We want from A -> B
    float specular      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambientVec     = attenuation * vec3(texture(material.diffuse, fTexCoords)) * light.ambient;
    vec3 diffuseVec     = attenuation * vec3(texture(material.diffuse, fTexCoords)) * diffuse * light.diffuse;
    vec3 specularVec    = attenuation * vec3(texture(material.specular, fTexCoords)) * specular * light.specular;
    vec3 result = (ambientVec + diffuseVec + specularVec) * light.color.rgb;
    return result;
}