#version 330 core
out vec4 FragColor;

uniform ivec2 resolution;
uniform float zoom;
uniform vec2 pos;
uniform vec2 transpose;
uniform vec2 pos_old;


float function(in float x, in float y) {
  return abs(-1.0f * x * x - y);
}

void main() {
  vec3 color = vec3(0.0f);
  float linewidth = 0.001f * zoom;

  // define coordinates
  vec2 coord = (zoom * (2.0 * gl_FragCoord.xy - resolution) / resolution) + transpose;

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

  // draw function 
  float score = function(coord.x, coord.y);
  if(score < 0.003 * zoom) {
    color = vec3(1.0f, 0.94f, 0.0f);
  }
  else if(score < 0.005 * zoom) {
    color = vec3(0.63f, 0.59f, 0.15f);
  }

  // draw line segment
  vec2 dir = pos - pos_old;
  float lDir = length(dir);
  vec2 normDir = normalize(dir);
  vec2 toPoint = coord - pos;
  float lToPoint = length(toPoint);
  if ((abs(dot(normDir, toPoint) - lToPoint) < 0.05f * zoom) && lToPoint < lDir) {
    color = vec3(0.0f, 0.0f, 1.0f);
  }




  // draw pos
  if (distance(pos, coord) < 0.05f) {
    color = vec3(1.0f, 0.0f, 0.0f);
  }
  FragColor = vec4(color, 1.0f);

}
