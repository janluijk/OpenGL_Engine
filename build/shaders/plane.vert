#version 330 core
layout (location = 0) in vec3 position;

out vec4 worldPos;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    worldPos = model * vec4(position, 1.0);
    gl_Position = projection * view * worldPos;
}