#version 330 core

// This has to be matched to the binders attribpointers in the C program
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs;
layout(location = 2) in vec4 color;

out vec4 particleColor;
out vec2 UV;

uniform mat4 ProjectionMatrix;
uniform mat4 WTVMatrix;

void main(void)
{
	float particleSize = xyzs.w; // Last variable in vec
	vec3 particleCenter_wordspace = xyzs.xyz;

  vec3 vertexPosition_worldspace =
    vec3(squareVertices.x * particleSize,
          squareVertices.y * particleSize,
          squareVertices.z * particleSize);

  // Translate to particle world position
  mat4 translation = mat4(1,0,0,0,
                           0,1,0,0,
                           0,0,1,0,
                           particleCenter_wordspace.x, particleCenter_wordspace.y, particleCenter_wordspace.z, 1);


  mat4 modelView = WTVMatrix * translation;

  // Turn off rotation for billboarding effect:
  //Col1
  modelView[0][0] = 1.0;
  modelView[0][1] = 0.0;
  modelView[0][2] = 0.0;
  //Col2
  modelView[1][0] = 0.0;
  modelView[1][1] = 1.0;
  modelView[1][2] = 0.0;
  //Col3
  modelView[2][0] = 0.0;
  modelView[2][1] = 0.0;
  modelView[2][2] = 1.0;

	// Output position of the vertex
  gl_Position = ProjectionMatrix * modelView * vec4(vertexPosition_worldspace, 1.0f);

	UV = squareVertices.xy + vec2(0.5, 0.5);
	particleColor = clamp(color, 0, 1);
}
