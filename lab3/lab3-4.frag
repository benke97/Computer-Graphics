#version 150

out vec4 out_Color;
in vec2 outTexCoord;
uniform sampler2D texUnit;
in vec3 inNormal;
in vec3 out_Position;
uniform vec3 camPos;

uniform vec3 lightSourcesDirPosArr[4];

uniform vec3 lightSourcesColorArr[4];

uniform float specularExponent;

uniform bool isDirectional[4];

void main(void)
{
	vec3 ambient = vec3(0.1,0.1,0.1);
	float shade = 0.0f;
	vec3 color = vec3(0,0,0);
	vec3 totcolor = vec3(0,0,0);
	vec3 light = vec3(0,0,0);

	float specStrength = 0.5f;
	float spec = 0.0f;
	//totcolor += ambient;
	for (int i = 0; i<= 3; i++)
	{
		if (isDirectional[i])
		{
			light = normalize(lightSourcesDirPosArr[i]);
			shade = dot(normalize(inNormal), light);
			shade = clamp(shade, 0, 1);
			color = shade*lightSourcesColorArr[i];
			totcolor += color;

		}
		else
		{
			light = normalize(lightSourcesDirPosArr[i] - out_Position);
			shade = dot(normalize(inNormal), light);
			shade = clamp(shade, 0, 1);
			color = shade*lightSourcesColorArr[i];
			totcolor += color;
		}

		// Specular shade
		vec3 r = reflect(-light, inNormal);
		//vec3 r = 2*inNormal*dot(-light, inNormal) + light;
		vec3 eyeDir = normalize(camPos-out_Position);
		if ( dot(light, inNormal) > 0.0)
		{
			spec = dot(r, eyeDir);
			float exp = specularExponent;
			spec = max(spec, 0.01);
			spec = pow(spec, specularExponent);
			color = spec*lightSourcesColorArr[i];
					totcolor += color;
		}
	}

	totcolor = clamp(totcolor, 0, 1);
	vec4 textemp = texture(texUnit, outTexCoord);
	out_Color = vec4(totcolor.x*textemp.x, totcolor.y*textemp.y, totcolor.z*textemp.z, 1);
}
