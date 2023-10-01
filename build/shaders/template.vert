#version 400 core
layout (location = 0) in vec3 vPos;

// Out

void main()
{
    gl_Position = vec4(vPos, 1.0);
}

