#version 150

out vec4 out_Color;
in vec4 vertcolor;
in vec2 outTexCoord;
uniform float t;
uniform sampler2D texUnit;

void main(void)
{
	float time = sin(t/1000);
	out_Color = texture(texUnit, outTexCoord);
}
