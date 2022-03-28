#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormalVec;
layout (location = 2) in vec2 vTexCoords;

// Out
out vec3 fNormalVec;
out vec3 fFragPos;
out vec2 fTexCoords;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalTransform;

void main()
{
    fFragPos = vec3(model * vec4(vPos, 1.0));
    fTexCoords = vTexCoords;
    fNormalVec = normalize(normalTransform * vNormalVec);
    gl_Position = projection * view * vec4(fFragPos, 1.0);
}