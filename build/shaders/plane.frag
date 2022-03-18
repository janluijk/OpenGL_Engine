#version 330 core
in vec4 worldPos;

out vec4 FragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    int even = int(worldPos.x) + int(worldPos.z);
    if(even % 2 == 0)
    {
        FragColor = vec4(1.0);
    }
    else 
    {
        FragColor = vec4(0.8, 0.8, 0.8, 1.0);
    }
}       