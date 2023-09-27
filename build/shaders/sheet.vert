#version 400 core
layout (location = 0) in vec3 vPos;

// Out
out vec3 fFragPos; // World Coordinate

void main()
{
    fFragPos = vPos;
    gl_Position = vec4(fFragPos, 1.0);
}
