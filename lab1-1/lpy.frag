#version 150

uniform sampler2D texUnit;
uniform float texSize;
in vec2 texCoord;
out vec4 fragColor;

void main(void){
  float offset = 1.0/512;
  vec2 tc = texCoord;
  vec4 c = texture(texUnit, tc);
  tc.y = tc.y + offset;
  vec4 l = texture(texUnit, tc);
  tc.y = tc.y - 2.0*offset;
  vec4 r = texture(texUnit, tc);
  tc.y = tc.y -offset;
  fragColor = (c+c+r+l)*0.25;

}
