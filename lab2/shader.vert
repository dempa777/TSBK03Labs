#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
uniform mat4 matrix;
uniform mat4 rotBoneOne;
uniform mat4 rotBoneTwo;
uniform mat4 posBoneOne;
uniform mat4 posBoneTwo;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning på GPU
//
// Flytta över din implementation av soft skinning från CPU-sidan
// till vertexshadern. Mer info finns på hemsidan.

void main(void)
{
  /*
  mat4 minusTransBoneOne = posBoneOne;
  minusTransBoneOne[3].x = -minusTransBoneOne[3].x;
  minusTransBoneOne[3].y = -minusTransBoneOne[3].y;
  minusTransBoneOne[3].z = -minusTransBoneOne[3].z;
  mat4 minusTransBoneTwo = posBoneTwo;
  minusTransBoneTwo[3].x = -minusTransBoneTwo[3].x;
  minusTransBoneTwo[3].y = -minusTransBoneTwo[3].y;
  minusTransBoneTwo[3].z = -minusTransBoneTwo[3].z;

  vec4 bone1 = posBoneOne*rotBoneOne*minusTransBoneOne*vec4(in_Position, 1.0);
  vec4 bone2 = posBoneTwo*rotBoneTwo*minusTransBoneTwo*vec4(in_Position, 1.0);

  vec4 result = in_TexCoord.x*bone1 + in_TexCoord.y*bone2;*/
// transformera resultatet med ModelView- och Projection-matriserna
	//gl_Position = matrix * result;

gl_Position = matrix * vec4(in_Position, 1.0);

	// sätt röd+grön färgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// Lägg på en enkel ljussättning på vertexarna 	
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}

