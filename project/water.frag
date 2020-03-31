#version 150

out vec4 out_Color;
in vec3 fragnormal;
in vec3 outPosition;
uniform vec3 camPos;

void main(void)
{
  float spec = 0.0f;
  vec3 ambient = vec3(0.8,0.8,0.8);
  vec3 light = vec3(0.58, 0.58, 0.58);
  float shade = dot(light, normalize(fragnormal));
  shade = clamp(shade, 0, 1);
  vec3 color = vec3(0,0,0);

  vec3 r = 2*fragnormal*dot(light, fragnormal) - light;
  vec3 eyeDir = normalize(camPos-outPosition);
  if ( dot(light, fragnormal) > 0.0)
  {
    spec = dot(r, eyeDir);
    float exp = 5;
    spec = max(spec, 0.01);
    spec = pow(spec, exp);
    color = spec*vec3(1,1,1);
    color +=ambient;
    out_Color = vec4(shade*color.x*0,shade*color.y*0.467,shade*color.z*0.745,0.9);
  }
  else
  {
  out_Color = vec4(0,0.467,0.745,0.9);
  }
}
