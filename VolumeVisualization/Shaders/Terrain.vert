#version 130
#extension GL_EXT_gpu_shader4 : enable

in vec2 aPosition;
in vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

uniform sampler2D uHeightMap;
uniform sampler2D uBathyMap;

uniform float uLayerWidth;
uniform vec3 uLightPosition;

out vec3 vLight;
out vec2 vTexCoords;
out vec4 vScreenPosition;


void main()
{
	vTexCoords = (aPosition.xy) / (uLayerWidth + 1.0) + vec2(0.5);

	vec4 Position = vec4(aPosition.x, texture(uHeightMap, vTexCoords).r - (1 - texture(uBathyMap, vTexCoords).r) * 0.6, aPosition.y, 1);
	Position.y *= 75.0;
	gl_ClipDistance[0] = dot(vec4(0,1,0,0), Position);

	gl_Position = vScreenPosition = uProjMatrix * uViewMatrix * uModelMatrix * Position;

	vec3 LightPosition = uLightPosition;
	vLight = normalize(LightPosition - vec3(uModelMatrix * Position));
}
