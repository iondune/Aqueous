#version 330

uniform mat4 mvp;

in vec2 position;
in vec2 texcoord;
in vec4 color;

out vec2 TextureCoordinates;
out vec4 Color;

void main()
{
	gl_Position = mvp * vec4(position, 0.0, 1.0);
	TextureCoordinates = texcoord;
	Color = color;
}
