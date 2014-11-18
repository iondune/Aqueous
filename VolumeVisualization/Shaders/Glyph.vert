
#version 150

in vec3 vPosition;
in vec3 vColor;

out vec3 geColor;


void main()
{
	gl_Position = vec4(vPosition, 1.0);
	geColor = vColor;
}
