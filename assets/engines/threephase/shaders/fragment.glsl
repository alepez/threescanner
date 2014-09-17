#version 330 core

out vec3 color;

uniform vec2 resolution;
uniform float phase = 0.0;
uniform int orientation = 0;

const float twopi = 6.283185;
const float width = 32.0;
const int test = 0;
const float testGrid = 16.0;

void pattern(float x) {
  float p = (phase - 1.0) / 3.0;
  float t = x / width;
  float v = 0.5 + cos((t + p) * twopi) * 0.5;
  color = vec3(v, v, v);
}

void doTest() {
  float x = gl_FragCoord.x / resolution.x;
  float y = gl_FragCoord.y / resolution.y;
  float xDist = 1.0 / 16.0;
  float yDist = 1.0 / 16.0 * resolution.x / resolution.y;
  color = vec3(mod(x, xDist) / xDist, mod(y, yDist) / yDist, 1.0);
}

void main() {
  if (test == 1) {
    doTest();
    return;
  }
  if (orientation == 0) {
    pattern(gl_FragCoord.y);
  }
  else {
    pattern(gl_FragCoord.x);
  }
}
