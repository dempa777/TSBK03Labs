#version 150

uniform sampler2D texUnit;
uniform float texSize;
in vec2 texCoord;
out vec4 fragColor;

void main(void){
  float offset = 2.0/512;
  vec2 tc = texCoord;
  vec4 c = texture(texUnit, tc);
  tc.x = tc.x + offset;
  vec4 l = texture(texUnit, tc);
  tc.x = tc.x - 2.0*offset;
  vec4 r = texture(texUnit, tc);
  tc.x = tc.x -offset;
  fragColor = (c+c+r+l)*0.25;

}
