#version 330 core
out vec4 FragColor;

uniform ivec2 resolution;
uniform float zoom;
uniform vec2 position;

float function(in float x, in float y) {
  return abs(x * x - y);
}

void main() {
  vec3 color = vec3(0.0f);
  float linewidth = 0.001f * zoom;

  // define coordinates
  vec2 coord = zoom * (2.0 * gl_FragCoord.xy - resolution) / resolution;

  // draw gridlines
  vec2 intDist = abs(coord - round(coord));

  if(intDist.x < linewidth) {
    color = vec3(0.5f, 0.6f, 0.8f);
  }
  if(intDist.y < linewidth) {
    color = vec3(0.5f, 0.6f, 0.8f); 
  }
  if(intDist.x < linewidth * 1.5f && abs(round(coord.x)) < 0.1f) {
    color = vec3(1.0f);
  }
  if(intDist.y < linewidth * 1.5f && abs(round(coord.y)) < 0.1f) {
    color = vec3(1.0f);
  }

  // draw position
  if (distance(position, coord) < 0.05f) {
    color = vec3(1.0f, 0.0f, 0.0f);
  }



  // draw function 
  float score = function(coord.x, coord.y);
  if(score < 0.003 * zoom) {
    color = vec3(1.0f, 0.94f, 0.0f);
  }
  else if(score < 0.005 * zoom) {
    color = vec3(0.63f, 0.59f, 0.15f);
  }
  FragColor = vec4(color, 1.0f);
}
