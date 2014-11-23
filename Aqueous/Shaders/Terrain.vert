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
uniform int uHeightInterpolationMode;

out vec3 vLight;
out vec2 vTexCoords;
out vec4 vScreenPosition;
out float vHeight;


float BicubicInterpolateHeight(sampler2D Texture, float Offset)
{
	return
		(texture(Texture, vTexCoords + vec2( Offset,  Offset)).r  +
		(texture(Texture, vTexCoords + vec2( Offset, -Offset)).r) +
		(texture(Texture, vTexCoords + vec2(-Offset,  Offset)).r) +
		(texture(Texture, vTexCoords + vec2(-Offset, -Offset)).r)) / 4.0;
}

float Height(sampler2D Texture)
{
	return texture(Texture, vTexCoords).r;
}

void main()
{
	const vec3 LightPosition = vec3(10.0, 10.0, 10.0);
	const float MaxTerrainElevation = 75.0;
	const float MaxBathyDepth = 45.0;

	vTexCoords = (aPosition.xy) / (uLayerWidth + 1.0) + vec2(0.5);

	float Offset = 1.0 / uLayerWidth;
	vec4 Position = vec4(aPosition.x, 0, aPosition.y, 1);

	if (uHeightInterpolationMode == 1)
		Position.y = MaxTerrainElevation * Height(uHeightMap) - MaxBathyDepth * (1.0 - BicubicInterpolateHeight(uBathyMap, Offset));
	else if (uHeightInterpolationMode == 2)
		Position.y = MaxTerrainElevation * BicubicInterpolateHeight(uHeightMap, Offset) - MaxBathyDepth * (1.0 - BicubicInterpolateHeight(uBathyMap, Offset));
	else // if (uHeightInterpolationMode == 0)
		Position.y = MaxTerrainElevation * Height(uHeightMap) - MaxBathyDepth * (1.0 - Height(uBathyMap));

	vHeight = (Position.y + MaxBathyDepth) / (MaxTerrainElevation + MaxBathyDepth);
	vLight = normalize(LightPosition - vec3(uModelMatrix * Position));

	gl_ClipDistance[0] = dot(vec4(0,1,0,0), Position);
	gl_Position = vScreenPosition = uProjMatrix * uViewMatrix * uModelMatrix * Position;
}
