#version 400 core
out vec4 FragColor;

uniform ivec2 resolution;
uniform float zoom;
uniform vec2 pos;
uniform vec2 transpose;
uniform vec2 pos_old;

float f(in float x, in float y) { return (x * x - y); }

vec3 drawFunction(in vec3 color, in vec2 coord) {
  float score = abs(f(coord.x, coord.y));

  if(score < 0.003 * zoom) {
    color = vec3(1.0f, 0.94f, 0.0f);
  }
  else if(score < 0.005 * zoom) {
    color = vec3(0.63f, 0.59f, 0.15f);
  }
  return color;
}

vec2 getCoord() {
  vec2 coord = (zoom * (2.0 * gl_FragCoord.xy - resolution) / resolution) + transpose;
  coord.x = coord.x * float(resolution.x) / float(resolution.y);
  return coord;
}

vec3 drawGridlines(in vec3 color, in vec2 coord, in float linewidth) {
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
  return color;
}






// MAIN FUNC
void main() {
  vec3 color = vec3(0.0f);
  float linewidth = 0.001f * zoom;
  
  // Obtain screen coordinate
  vec2 coord = getCoord();

  // Gridlines
  color = drawGridlines(color, coord, linewidth);

  // Display function
  color = drawFunction(color, coord);
  
  FragColor = vec4(color, 1.0f);
}
