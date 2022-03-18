#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 aNormalVec;

out vec3 normalVec;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;

void main()
{
    vec3 vec = vec3(model *  vec4(vertexPos, 1.0));
    fragPos = vec;
    normalVec = normal * aNormalVec;
    gl_Position = projection * view * vec4(vec, 1.0);
}