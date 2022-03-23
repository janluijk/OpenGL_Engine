#version 400 core
out vec4 FragColor;


// STRUCTS
struct Material {
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

vec3 calcPointLight(PointLight pointlight, vec3 fragPos, vec3 fNormalVec, vec3 viewDir);

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


vec3 calcPointLight(PointLight pointlight, vec3 fragPos, vec3 fNormalVec, vec3 viewDir) 
{   
    
    vec3 planeColor = vec3(0.8, 0.8, 0.8);
    if ((int(fragPos.x) + int(fragPos.z)) % 2 == 0) {
        planeColor = vec3(0.5, 0.2, 0.2);
    }
    // Distance light and cube
    float lightDistance = length(pointlight.position - fWorldPos);
    float attenuation = 1.0 / pointlight.ambient.r * lightDistance;
    if(pointlight.quadratic == 0.0) {
        attenuation = 1.0 / (lightDistance * lightDistance);
    }

    // Diffuse
    vec3 rayDir         = normalize(pointlight.position - fragPos); // A - B means B -> A 
    float diffuse       = max(dot(fNormalVec, rayDir), 0.0);
    
    // Specular 
    vec3 reflectDir     = reflect(-rayDir, fNormalVec); // We want from A -> B
    float specular      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambientVec     = attenuation * planeColor * pointlight.ambient;
    vec3 diffuseVec     = attenuation * planeColor * diffuse * pointlight.diffuse;
    vec3 specularVec    = specular * pointlight.specular;
    vec3 result = (ambientVec + diffuseVec + specularVec) * pointlight.color.rgb;
    return result;
}