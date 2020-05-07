#version 150

in vec4 vertcolor;
in vec2 outTexCoord;
in vec3 fragNormal;
in vec3 out_position;

out vec4 out_Color;

uniform float t;
uniform float heat;
uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(fragNormal), light);
	shade = clamp(shade, 0, 1);
	//float time = sin(t/1000);
	//out_Color = shade*texture(texUnit, outTexCoord);
	vec4 coloor = vec4(0.0,0.0,0.0,0.0);
	vec3 overheat = vec3(1.0,1.0,1.0);
	if (out_position.z > 20){
		overheat.x = heat/10;
		if (overheat.x < 1.0)
		{
		overheat.x = 1.0;
		}
	}
	vec4 textur = texture(texUnit, outTexCoord);
	out_Color = vec4(textur.x*overheat.x,textur.y*overheat.y,textur.z*overheat.z,1.0);
  //out_Color = vec4(1.0);
}
