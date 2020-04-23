#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square

// Output data ; will be interpolated for each fragment.
out vec4 particleColor;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRightWorldSpace;
uniform vec3 CameraUpWorldSpace;
uniform mat4 ProjectionMatrix;
uniform mat4 WTVMatrix;
uniform mat4 Trans;

void main(void)
{
	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = xyzs.xyz;

	vec3 vertexPosition_worldspace =
		particleCenter_wordspace
		+ squareVertices * particleSize;

	// Output position of the vertex
	gl_Position = ProjectionMatrix * WTVMatrix * Trans * vec4(squareVertices, 1.0f);


	particleColor = color;
}
