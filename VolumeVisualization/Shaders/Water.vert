#version 130
#extension GL_EXT_gpu_shader4 : enable

in vec2 aPosition;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

uniform sampler2D uHeightMap;
uniform float uLayerWidth;
uniform vec3 uLightPosition;

out vec3 vLight;
out vec2 vTexCoords;
out vec4 vScreenPosition;


void main()
{
	vTexCoords = (aPosition.xy) / (uLayerWidth + 1.0) + vec2(0.5);

	vec4 Position = vec4(aPosition.x, texture(uHeightMap, vTexCoords).r * 0.1, aPosition.y, 1);
	Position.y *= 75.0;

	gl_Position = vScreenPosition = uProjMatrix * uViewMatrix * uModelMatrix * Position;

	vec3 LightPosition = uLightPosition;
	vLight = normalize(LightPosition - vec3(uModelMatrix * Position));
}
