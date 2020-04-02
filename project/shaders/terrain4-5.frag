#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;
uniform sampler2D dirttex;
in vec3 fragnormal;
in vec3 outPosition;

void main(void)
{
	vec3 light = vec3(0.58, 0.58, 0.58);
	float shade = dot(light, normalize(fragnormal));
	shade = clamp(shade, 0, 1);
	outColor = shade*texture(dirttex, texCoord);
}
