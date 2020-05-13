#version 330 core

out vec4 outColor;

in vec2 texCoord;
in vec3 fragnormal;
in vec3 outPosition;

//uniform sampler2D tex;
uniform sampler2D terrain_texture;

//For light Ball:
uniform int LightBallsQuantity;
uniform vec3 lightBallsPositions[100];
uniform vec3 lightBallsColor[100];
uniform float lightBallsIntensities[100];

uniform vec3 camPos;

// For flashlight:
uniform vec3 flashlightPosition;
uniform vec3 flashlightDirection;
uniform float flashlightCutOff;
uniform float flashlightOuterCutOff;
uniform bool toggleFlashLight;

//For laser:
uniform int LaserQuantity;
uniform vec3 laserPositions[100];
uniform vec3 laserColors[100];
uniform float laserIntensities[100];

//For Flares:
uniform int FlaresQuantity;
uniform vec3 FlaresPositions[100];
uniform vec3 FlaresColor[100];
uniform float FlaresIntensities[100];



void main(void)
{
	vec3 ambient = vec3(0.1,0.1,0.1);
	float shade = 0.0f;
	vec3 color = vec3(0,0,0);
	vec3 totcolor = vec3(0,0,0);
	vec3 light = vec3(0,0,0);
	float dist = 1.0f;

	float specStrength = 0.5f;
	float spec = 0.0f;
	totcolor += ambient;


	// --------- FlashLight ------------
	if (toggleFlashLight)
	{
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
	}

	// outercolor


	// ---------------------------------

	for (int i = 0; i < LightBallsQuantity; i++)
	{

		dist = distance(lightBallsPositions[i], outPosition) * 4;
		light = normalize(lightBallsPositions[i] - outPosition)/dist*lightBallsIntensities[i];
		shade = dot(normalize(fragnormal), light);

		shade = clamp(shade, 0, 1);
		color = shade*lightBallsColor[i];
		totcolor += color;

	}

	//Laser
	for (int i = 0; i < LaserQuantity; i++)
	{
		dist = distance(laserPositions[i], outPosition) * 4;
		light = normalize(laserPositions[i] - outPosition)/dist;
		shade = dot(normalize(fragnormal), light);

		shade = clamp(shade, 0, 1);
		color = shade*laserColors[i];
		totcolor += color;

	}
	// ---------------------------------
	for (int i = 0; i < FlaresQuantity; i++)
	{

		dist = distance(FlaresPositions[i], outPosition) * 4;
		light = normalize(FlaresPositions[i] - outPosition)/dist*FlaresIntensities[i];
		shade = dot(normalize(fragnormal), light);

		shade = clamp(shade, 0, 1);
		color = shade*FlaresColor[i];
		totcolor += color;
	}

	// ---------------------------------



	totcolor = clamp(totcolor, 0, 1);
	vec4 textemp = texture(terrain_texture, texCoord);
	outColor = vec4(totcolor.x*textemp.x, totcolor.y*textemp.y, totcolor.z*textemp.z, 1);

}
