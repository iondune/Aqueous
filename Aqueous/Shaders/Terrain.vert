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


const float MaxTerrainElevation = 75.0;
const float MaxBathyDepth = 45.0;

float BicubicInterpolateHeight(sampler2D Texture)
{
	float PixelWidth = 1.0 / uLayerWidth;

	return
		(texture(Texture, vTexCoords + vec2( PixelWidth,  PixelWidth)).r  +
		(texture(Texture, vTexCoords + vec2( PixelWidth, -PixelWidth)).r) +
		(texture(Texture, vTexCoords + vec2(-PixelWidth,  PixelWidth)).r) +
		(texture(Texture, vTexCoords + vec2(-PixelWidth, -PixelWidth)).r)) / 4.0;
}

float Height(sampler2D Texture)
{
	return texture(Texture, vTexCoords).r;
}

float GetHeight()
{
	if (uHeightInterpolationMode == 1)
		return MaxTerrainElevation * Height(uHeightMap) - MaxBathyDepth * (1.0 - BicubicInterpolateHeight(uBathyMap));
	else if (uHeightInterpolationMode == 2)
		return MaxTerrainElevation * BicubicInterpolateHeight(uHeightMap) - MaxBathyDepth * (1.0 - BicubicInterpolateHeight(uBathyMap));
	else // if (uHeightInterpolationMode == 0)
		return MaxTerrainElevation * Height(uHeightMap) - MaxBathyDepth * (1.0 - Height(uBathyMap));
}

void main()
{
	const vec3 LightPosition = vec3(10.0, 10.0, 10.0);

	vTexCoords = (aPosition.xy) / (uLayerWidth + 1.0) + vec2(0.5);

	vec4 Position = vec4(aPosition.x, GetHeight(), aPosition.y, 1);

	vHeight = (Position.y + MaxBathyDepth) / (MaxTerrainElevation + MaxBathyDepth);
	vLight = normalize(LightPosition - vec3(uModelMatrix * Position));

	gl_ClipDistance[0] = dot(vec4(0,1,0,0), Position);
	gl_Position = vScreenPosition = uProjMatrix * uViewMatrix * uModelMatrix * Position;
}
