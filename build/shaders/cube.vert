#version 330 core
layout (location = 0) in vec3 vVertexPos;
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
uniform mat3 normal;

void main()
{
    vec3 vec = vec3(model *  vec4(vVertexPos, 1.0));
    fFragPos = vec;
    fTexCoords = vTexCoords;
    fNormalVec = normal * vNormalVec;
    gl_Position = projection * view * vec4(vec, 1.0);
}