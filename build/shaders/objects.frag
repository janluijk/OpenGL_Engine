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

#define NR_POINT_LIGHTS 3

in vec3 fNormalVec;
in vec3 fFragPos;
in vec2 fTexCoords;

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLight;

vec3 calcDirectionalLight(DirectionalLight light, vec3 fNormalVec, vec3 viewDir, vec3 diffuseTexture, vec3 specularTexture);
vec3 calcPointLight(PointLight light, vec3 fragPos, vec3 fNormalVec, vec3 viewDir, vec3 diffuseTexture, vec3 specularTexture);

// Main
void main()
{
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fFragPos);

    // Texture sampling
    vec3 diffuseTexture = vec3(texture(material.diffuse, fTexCoords));
    vec3 specularTexture = vec3(texture(material.specular, fTexCoords));
    
    // Directional light
    result = calcDirectionalLight(directionalLight, fNormalVec, viewDir, diffuseTexture, specularTexture);

    // Pointlights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) 
    {
        result += calcPointLight(pointLights[i], fFragPos, fNormalVec, viewDir, diffuseTexture, specularTexture);
    }

    FragColor           = vec4(result, 1.0);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 fNormalVec, vec3 viewDir, vec3 diffuseTexture, vec3 specularTexture)
{
    vec3 rayDir     = normalize(-light.direction);
    float diffuse   = max(dot(rayDir, fNormalVec), 0.0);

    vec3 reflectDir = reflect(-rayDir, fNormalVec);
    float specular  = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambientVec     = light.ambient * diffuseTexture;
    vec3 diffuseVec     = diffuse * light.diffuse * diffuseTexture;
    vec3 specularVec    = specular * light.specular * specularTexture;

    vec3 result = (ambientVec + diffuseVec + specularVec) * light.color.rgb;
    return result;
}

vec3 calcPointLight(PointLight light, vec3 fragPos, vec3 fNormalVec, vec3 viewDir, vec3 diffuseTexture, vec3 specularTexture)
{   
    // LIGHT   
    vec3 rayDir         = normalize(light.position - fragPos); // A - B means B -> A 
    float diffuse       = max(dot(fNormalVec, rayDir), 0.0);
    
    vec3 reflectDir     = reflect(-rayDir, fNormalVec); // We want from A -> B
    float specular      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambientVec     = light.ambient * diffuseTexture;
    vec3 diffuseVec     = diffuse * light.diffuse * diffuseTexture;
    vec3 specularVec    = specular * light.specular * specularTexture;

    // DISTANCE
    float lightFragDist     = length(light.position - fragPos);
    float playerFragDist    = length(viewPos - fragPos);
    float totalDistance     = lightFragDist + 0.1 * playerFragDist;
    float attenuation       = 1.0 / (light.constant + light.linear * (totalDistance + light.quadratic * totalDistance));

    //            LIGHT                                                     * DISTANCE
    vec3 result = (ambientVec + diffuseVec + specularVec) * light.color.rgb * attenuation;
    return result; 
}