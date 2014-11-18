
#version 150

uniform mat4 Model;

in vec3 vPosition;
in vec3 vColor;

out vec3 geColor;


void main()
{
	gl_Position = Model * vec4(vPosition, 1.0);
	geColor = vColor;
}
