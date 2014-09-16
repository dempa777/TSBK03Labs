#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
  vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates

  // Calculate gradients here
  float offset = 1.0 / 256.0; // texture size, same in both directions

  mat3 mvt = mat3(Ps, Pt, out_Normal);

  vec3 texLight = mvt*light;

  vec2 tcs = vec2(outTexCoord.s + offset, outTexCoord.t);
  float bs = texture(texUnit, tcs) - texture(texUnit, outTexCoord); 
  vec2 tct = vec2(outTexCoord.s , outTexCoord.t+ offset);
  float bt = texture(texUnit, tct) - texture(texUnit, outTexCoord);
  
  vec3 Pss = mvt*Ps;
  vec3 Ptt = mvt*Pt;

  vec3 normal = cross(Pss, Ptt) - Pss*bs - Ptt*bt;//normalize(texNormal + bs + bt);
  // Simplified lighting calculation.
  // A full solution would include material, ambient, specular, light sources, multiply by texture.
  out_Color = vec4( dot(normal, texLight));
}
