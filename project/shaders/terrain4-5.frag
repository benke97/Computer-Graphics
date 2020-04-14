#version 150

out vec4 outColor;

in vec2 texCoord;
in vec3 fragnormal;
in vec3 outPosition;

//uniform sampler2D tex;
uniform sampler2D dirttex;

//For ball:
uniform int LightBallsQuantity;
uniform vec3 camPos;
uniform vec3 lightBallsPositions[100];
uniform vec3 lightBallsColor[100];
uniform float lightBallsIntensities[100];
uniform float specularExponent;

// For flashlight:
uniform vec3 flashlightPosition;
uniform vec3 flashlightDirection;
uniform float flashlightCutOff;
uniform float flashlightOuterCutOff;


void main(void)
{
	vec3 ambient = vec3(0.2,0.2,0.2);
	float shade = 0.0f;
	vec3 color = vec3(0,0,0);
	vec3 totcolor = vec3(0,0,0);
	vec3 light = vec3(0,0,0);
	float dist = 1.0f;

	float specStrength = 0.5f;
	float spec = 0.0f;
	totcolor += ambient;


	// --------- FlashLight ------------
	vec3 flashDir 	= normalize(flashlightPosition - outPosition);

	float theta 		= dot(flashDir, normalize(-flashlightDirection));
	float epsilon   = flashlightCutOff - flashlightOuterCutOff;
	float intensity = clamp((theta - flashlightOuterCutOff) / epsilon, 0.0, 1.0);

	if (theta > flashlightCutOff)
	{
		dist = distance(flashlightPosition, outPosition);
		color = vec3(4/dist, 4/dist, 4/dist);


		color.x = clamp(color.x, 0, 0.8);
		color.y = clamp(color.y, 0, 0.8);
		color.z = clamp(color.z, 0, 0.8);

		totcolor += color;
	}
	if( theta > flashlightOuterCutOff)
	{
		if (theta < flashlightCutOff)
		{
			dist = distance(flashlightPosition, outPosition);
			color = vec3(4/dist, 4/dist, 4/dist);
			color.x = clamp(color.x, 0, 0.8);
			color.y = clamp(color.y, 0, 0.8);
			color.z = clamp(color.z, 0, 0.8);

			totcolor += intensity * color;
		}

	}


	// outercolor


	// ---------------------------------

	for (int i = 0; i < LightBallsQuantity; i++)
	{

		dist = distance(lightBallsPositions[i], outPosition) * 4;
		light = normalize(lightBallsPositions[i] - outPosition)/dist*lightBallsIntensities[i];
		//shade = abs(dot(normalize(fragnormal), light));
		shade = dot(normalize(fragnormal), light);

		shade = clamp(shade, 0, 1);
		color = shade*lightBallsColor[i];
		totcolor += color;

		/*
		// Specular shade
		vec3 r = reflect(-light, fragnormal);
		//vec3 r = 2*fragnormal*dot(light, fragnormal) - light;
		vec3 eyeDir = normalize(camPos - outPosition);
		if ( dot(light, fragnormal) > 0.0)
		{
			spec = dot(r, eyeDir);
			float exp = specularExponent;
			spec = max(spec, 0.01);
			spec = pow(spec, specularExponent);
			color = spec*lightBallsColor[i];
			totcolor += color;
		}
		*/

	}

	totcolor = clamp(totcolor, 0, 1);
	vec4 textemp = texture(dirttex, texCoord);
	outColor = vec4(totcolor.x*textemp.x, totcolor.y*textemp.y, totcolor.z*textemp.z, 1);

}
