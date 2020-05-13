#version 150

out vec4 out_Color;

uniform float intensity;

void main(void)
{
  out_Color = intensity*vec4(1.0);
}
