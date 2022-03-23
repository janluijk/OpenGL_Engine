#version 400 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormalVec;

// Out
out vec3 fWorldPos; //World Coordinate
out vec3 fNormalVec;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalTransform;

void main()
{
    fWorldPos = vec3(model * vec4(vPos, 1.0)); 
    fNormalVec = normalize(normalTransform * vNormalVec);
    gl_Position = projection * view * vec4(fWorldPos, 1.0);
}