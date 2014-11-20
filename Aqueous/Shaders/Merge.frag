#version 150
uniform sampler2D uSceneColor;
uniform sampler2D uRefractColor;

in vec2 vTexCoord;
out vec4 outColor;

void main()
{
	if (texture(uRefractColor, vTexCoord).rgb == vec3(0.0))
		outColor = texture(uSceneColor, vTexCoord);
	else
		outColor = texture(uRefractColor, vTexCoord);
}
