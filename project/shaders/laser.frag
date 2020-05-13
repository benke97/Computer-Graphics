#version 150

in vec4 vertcolor;
in vec2 outTexCoord;
in vec3 fragNormal;

out vec4 out_Color;

uniform float t;
uniform sampler2D texUnit;

void main(void)
{
  out_Color = vec4(1.0,0,0,0);
}
