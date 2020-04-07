#version 150

in vec4 vertcolor;
in vec2 outTexCoord;
in vec3 inNormal;

out vec4 out_Color;

uniform float t;
uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(inNormal), light);
	shade = clamp(shade, 0, 1);
	float time = sin(t/1000);
	//out_Color = shade*texture(texUnit, outTexCoord * 10);
  out_Color = vec4(1.0);
}
