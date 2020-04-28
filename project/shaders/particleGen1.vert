#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square

// Output data ; will be interpolated for each fragment.
out vec4 particleColor;
//out vec2 UV;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRightWorldSpace;
uniform vec3 CameraUpWorldSpace;
uniform vec3 CameraOutWorldSpace;
uniform mat4 ProjectionMatrix;
uniform mat4 WTVMatrix;

void main(void)
{
	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = xyzs.xyz;

	//vec3 vertexPosition_worldspace =
  //particleCenter_wordspace
  //+ CameraRightWorldSpace * squareVertices.x * particleSize
  //+ CameraUpWorldSpace * squareVertices.y * particleSize
  //+ CameraOutWorldSpace * squareVertices.z * particleSize;

  vec3 vertexPosition_worldspace =
    vec3(squareVertices.x * particleSize,
          squareVertices.y * particleSize,
          squareVertices.z * particleSize);

  mat4 ModelViewMod = mat4(1,0,0,0,
                           0,1,0,0,
                           0,0,1,0,
                           particleCenter_wordspace.x, particleCenter_wordspace.y, particleCenter_wordspace.z, 1);


  mat4 modelView2 = WTVMatrix * ModelViewMod;

  // Turn off rotation:
  //Col1
  modelView2[0][0] = 1.0;
  modelView2[0][1] = 0.0;
  modelView2[0][2] = 0.0;
//Col2
  modelView2[1][0] = 0.0;
  modelView2[1][1] = 1.0;
  modelView2[1][2] = 0.0;
//Col3
  modelView2[2][0] = 0.0;
  modelView2[2][1] = 0.0;
  modelView2[2][2] = 1.0;

	// Output position of the vertex
  gl_Position = ProjectionMatrix * modelView2 * vec4(vertexPosition_worldspace, 1.0f);


	//UV = squareVertices.xy + vec2(0.5, 0.5);
	particleColor = clamp(color, 0, 1);
}
