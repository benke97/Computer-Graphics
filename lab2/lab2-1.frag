#version 150

out vec4 out_Color;
in vec4 vertcolor;
in vec2 outTexCoord;
uniform float t;
uniform sampler2D texUnit;

void main(void)
{
	float time = abs(sin(t/100));
	float a = sin(outTexCoord.s * 30.0 + t/100)/2.0 + 0.5;
  float b = sin(outTexCoord.t * 30.0 * (1.0+sin(t/1000.0)))/2.0 + outTexCoord.s*0.5* (1.0+sin(t/500.0));
  out_Color = vec4(a, b, 0.3, 1.0); // inColor;
	//out_Color = vec4(time*outTexCoord,0.4,0.1);
}
