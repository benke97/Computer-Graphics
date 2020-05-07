#version 150

in vec4 vertcolor;
in vec2 outTexCoord;
in vec3 fragNormal;

out vec4 out_Color;

uniform float t;
uniform sampler2D texUnit;
uniform float LifeTimer;

void main(void)
{
	vec3 life = vec3(0.1 + (20 - LifeTimer)/20, 0.1, 0.1);
	//const vec3 light = vec3(0.58, 0.58, 0.58);
	//float shade;
	//shade = dot(normalize(fragNormal), light);
	//shade = clamp(shade, 0, 1);
	//float time = sin(t/1000);
	//out_Color = life * texture(texUnit, outTexCoord);
	vec4 textur = texture(texUnit, outTexCoord);

	out_Color = vec4(textur.x*life.x,textur.y*life.y,textur.z*life.z, 1.0);
  //out_Color = vec4(1.0);
}
