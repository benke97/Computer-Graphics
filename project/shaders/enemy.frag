#version 150

in vec4 vertcolor;
in vec2 outTexCoord;
in vec3 fragNormal;
in vec3 outPosition;

out vec4 out_Color;

uniform float t;
uniform sampler2D texUnit;
uniform float LifeTimer;

//For ball:
uniform int LightBallsQuantity;
uniform vec3 camPos;
uniform vec3 lightBallsPositions[100];
uniform vec3 lightBallsColor[100];
uniform float lightBallsIntensities[100];

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
	vec3 life = vec3(0.1 + (20 - LifeTimer)/20, 0.1, 0.1);

	float shade = 0.0f;
	vec3 color = vec3(0,0,0);
	vec3 totcolor = vec3(0,0,0);
	vec3 light = vec3(0,0,0);
	float dist = 1.0f;

	float specStrength = 0.5f;
	float spec = 0.0f;


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

	// ---------------------------------

	for (int i = 0; i < LightBallsQuantity; i++)
	{

		dist = distance(lightBallsPositions[i], outPosition) * 4;
		light = normalize(lightBallsPositions[i] - outPosition)/dist*lightBallsIntensities[i];
		shade = dot(normalize(fragNormal), light);

		shade = clamp(shade, 0, 1);
		color = shade*lightBallsColor[i];
		totcolor += color;


	}

	//Laser
	for (int i = 0; i < LaserQuantity; i++)
	{
		dist = distance(laserPositions[i], outPosition) * 4;
		light = normalize(laserPositions[i] - outPosition)/dist;
		shade = dot(normalize(fragNormal), light);

		shade = clamp(shade, 0, 1);
		color = shade*laserColors[i];
		totcolor += color;

	}
	// ---------------------------------
	for (int i = 0; i < FlaresQuantity; i++)
	{

		dist = distance(FlaresPositions[i], outPosition) * 4;
		light = normalize(FlaresPositions[i] - outPosition)/dist*FlaresIntensities[i];
		shade = dot(normalize(fragNormal), light);

		shade = clamp(shade, 0, 1);
		color = shade*FlaresColor[i];
		totcolor += color;
	}

	// ---------------------------------

	totcolor += life;
	totcolor = clamp(totcolor, 0, 1);

	vec4 textur = texture(texUnit, outTexCoord);
	/*if (dist > 1)
	{
		totcolor = vec3(0,0,1);
	}*/
	out_Color = vec4(textur.x*(totcolor.x), textur.y*(totcolor.y), textur.z*(totcolor.z), 1.0);
}
