#version 150

out vec4 out_Color;
in vec4 vertcolor;
in vec2 outTexCoord;
uniform float t;
uniform sampler2D texUnit;
in vec3 inNormal;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(inNormal), light);
	shade = clamp(shade, 0, 1);
	float time = sin(t/1000);
	out_Color = shade*texture(texUnit, outTexCoord * 10);
}
