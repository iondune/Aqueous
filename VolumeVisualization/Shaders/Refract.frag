#version 150

#define LIGHT_MAX 4

struct SMaterial
{
	vec3 AmbientColor;
	vec3 DiffuseColor;
};

struct SLight
{
	vec3 Color;
	float Radius;
};

in vec3 vLight[LIGHT_MAX];
in vec3 vNormal;
in vec2 vTexCoord;
in vec4 vScreenPosition;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform int uLightCount;
uniform SLight uLights[LIGHT_MAX];
uniform SMaterial uMaterial;

out vec4 outColor;

float sq(float v)
{
	return v * v;
}


void main()
{
	vec3 Diffuse = vec3(0);
	const vec2 OffsetScale = vec2(0.1);

	for (int i = 0; i < LIGHT_MAX && i < uLightCount; ++ i)
	{
		vec3 nLight = normalize(vLight[i]);
		vec3 nNormal = normalize(vNormal);
		vec3 Reflection = reflect(-nLight, nNormal);

		float Shading = clamp(dot(nNormal, nLight), 0.0, 1.0);
		float Distance = length(vLight[i]);
		float Attenuation = 1.0 / sq(Distance / uLights[i].Radius + 1);
		Diffuse += uMaterial.DiffuseColor * Shading * Attenuation * uLights[i].Color;
	}

	vec3 ShadedColor = Diffuse + uMaterial.AmbientColor;
	vec3 OffsetValue = texture(Texture0, vTexCoord).rgb - 0.5;
	vec2 SampleCoords = ((vScreenPosition.xy / vScreenPosition.w) + 1.0) / 2.0 + OffsetValue.rg*OffsetScale;

	if (SampleCoords.x >= 1.0)
		SampleCoords.x = 1 - (SampleCoords.x - 1);
	if (SampleCoords.y >= 1.0)
		SampleCoords.y = 1 - (SampleCoords.y - 1);

	if (SampleCoords.x <= 0.0)
		SampleCoords.x = -SampleCoords.x;
	if (SampleCoords.y <= 0.0)
		SampleCoords.y = -SampleCoords.y;

	vec3 BackgroundColor = texture(Texture1, SampleCoords).rgb;
	outColor = vec4(ShadedColor * 0.4 + BackgroundColor * 0.6, 1);
}
