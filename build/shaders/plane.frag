#version 330 core

// STRUCTS
struct Material {
    // If no textures are used, diffuse and specular
    // Are properties of the light only
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;
    vec4 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float c0;
    float c1;
    float c2;
};
uniform Light light;


/* CODE */
out vec4 FragColor;

in vec4 fWorldPos;
in vec3 fNormalVec;

// Uniforms
uniform vec3 viewPos;

void main()
{
     // Distance light and cube
    float lightDistance = length(light.position - fWorldPos.xyz); // lenght of vector from frag to light
    float attenuation = 1.0 / (light.c0 + lightDistance * (light.c1 + light.c2 * lightDistance)); // Brightness decreases by distance squared

    vec4 planeColor;
    int isEven = int(fWorldPos.x) + int(fWorldPos.z);
    if(isEven % 2 == 0)
    {
        planeColor = vec4(0.75, 0.75, 0.75, 1.0);
    }
    else 
    {
        planeColor = vec4(0.5, 0.1, 0.1, 1.0);
    }

    // Ambient  
    vec3 ambientVec     = light.ambient;

    // Diffuse
    vec3 rayDir         = normalize(light.position - fWorldPos.xyz);    // A - B means B -> A 
    float diffuse       = max(dot(fNormalVec, rayDir), 0.0);            // Diffuse -> [0, 1]
    vec3 diffuseVec     = diffuse * light.diffuse; 

    // specular 
    vec3 viewDir        = normalize(viewPos - fWorldPos.xyz);
    vec3 reflectDir     = reflect(-rayDir, fNormalVec); // We want from A -> B
    float specular      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularVec    = specular * light.specular;

    vec3 result         = (specularVec + ambientVec + diffuseVec) * planeColor.xyz * attenuation * light.color.rgb;
    FragColor           = vec4(result, 1.0);
}       