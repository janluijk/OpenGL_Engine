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

#define NR_POINT_LIGHTS 3

in vec3 fNormalVec;
in vec3 fFragPos;
in vec2 fTexCoords;

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLight;

vec3 calcDirectionalLight(DirectionalLight light, vec3 fNormalVec, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 fragPos, vec3 fNormalVec, vec3 viewDir);

// Main
void main()
{
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fFragPos);

    // directional light
    result = calcDirectionalLight(directionalLight, fNormalVec, viewDir);
    
    // Pointlights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) 
    {
        result += calcPointLight(pointLights[i], fFragPos, fNormalVec, viewDir);
    }

    // MATERIAL
    vec3 planeColor = vec3(0.8, 0.8, 0.8);
    if((int(fFragPos.x) + int(fFragPos.z)) % 2 == 0) {
        planeColor = vec3(0.9, 0.5, 0.5);
    }

    result = result * planeColor;
    FragColor = vec4(result, 1.0);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 fNormalVec, vec3 viewDir) 
{
    vec3 rayDir     = normalize(-light.direction);
    float diffuse   = max(dot(rayDir, fNormalVec), 0.0);

    vec3 reflectDir = reflect(-rayDir, fNormalVec);
    float specular  = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambientVec     = light.ambient;
    vec3 diffuseVec     = diffuse * light.diffuse;
    vec3 specularVec    = specular * light.specular;

    vec3 result = (ambientVec + diffuseVec + specularVec) * light.color.rgb;
    return result;
}

vec3 calcPointLight(PointLight light, vec3 fragPos, vec3 fNormalVec, vec3 viewDir) 
{   
    // LIGHT   
    vec3 rayDir         = normalize(light.position - fragPos); // A - B means B -> A 
    float diffuse       = max(dot(fNormalVec, rayDir), 0.0);
    
    vec3 reflectDir     = reflect(-rayDir, fNormalVec); // We want from A -> B
    float specular      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambientVec     = light.ambient;
    vec3 diffuseVec     = diffuse * light.diffuse;
    vec3 specularVec    = specular * light.specular;

    // DISTANCE
    float lightFragDist     = length(light.position - fragPos);
    float playerFragDist    = length(viewPos - fragPos);
    float totalDistance     = lightFragDist + playerFragDist;
    float attenuation       = 1.0 / (light.constant + light.linear * (totalDistance + light.quadratic * totalDistance));

    //            LIGHT                                                     * DISTANCE
    vec3 result = (ambientVec + diffuseVec + specularVec) * light.color.rgb * attenuation;
    return result; 
}