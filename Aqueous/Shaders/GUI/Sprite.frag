#version 330

uniform sampler2D tex;

in vec2 TextureCoordinates;
in vec4 Color;

out vec4 FragColor;

void main()
{
	FragColor = Color * texture2D(tex, TextureCoordinates);
}
