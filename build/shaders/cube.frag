#version 400 core
out vec4 FragColor;

uniform float zoom;
uniform float scale;
uniform vec2 pos;
uniform vec2 transpose;
uniform vec3 color;

void main() {
  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}

