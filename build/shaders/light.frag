#version 400 core

out vec4 FragColor;

// Uniforms
uniform vec4 lightColor;

void main()
{
    FragColor = lightColor;
}       