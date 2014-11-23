#version 130

in vec3 vLight;
in vec2 vTexCoords;
in vec4 vScreenPosition;
in float vHeight;

uniform sampler2D uColorMap;
uniform sampler2D uHeightMap;
uniform sampler2D uNormalMap;

uniform int uDebugHeight;
uniform int uDebugMode;
uniform float uLayerWidth;

out vec4 gl_FragData[2];


float getHeightAt(vec2 Offset)
{
	return texture(uHeightMap, vTexCoords + Offset);
}

vec3 getNormalAt(vec2 Offset)
{
	return texture(uNormalMap, vTexCoords + Offset).rgb * vec3(2.0) - vec3(1.0);
}

float getColorAt(vec2 Offset)
{
	return texture(uColorMap, vTexCoords + Offset).g;
}

float maxabs(vec3 v)
{
	return max(abs(v.x), max(abs(v.y), abs(v.z)));
}

float getOcclusion(float Offset)
{
	float occlusion = 0;
	float here = getHeightAt(vec2(0, 0));
	float step;

	step = getHeightAt(vec2(Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(-Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(0, Offset));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(0, -Offset));
	if (step > here)
		occlusion += step - here;

	Offset *= 2;
	step = getHeightAt(vec2(Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(-Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(0, Offset));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(0, -Offset));
	if (step > here)
		occlusion += step - here;

	Offset *= 2;
	step = getHeightAt(vec2(Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(-Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(0, Offset));
	if (step > here)
		occlusion += step - here;
	step = getHeightAt(vec2(0, -Offset));
	if (step > here)
		occlusion += step - here;
	return occlusion;
}

float getHighFrequencyOcclusion(float Offset)
{
	float occlusion = 0;
	float here = getColorAt(vec2(0, 0));
	float step;

	step = getColorAt(vec2(Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(-Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(0, Offset));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(0, -Offset));
	if (step > here)
		occlusion += step - here;

	Offset *= 2;
	step = getColorAt(vec2(Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(-Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(0, Offset));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(0, -Offset));
	if (step > here)
		occlusion += step - here;

	Offset *= 2;
	step = getColorAt(vec2(Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(-Offset, 0));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(0, Offset));
	if (step > here)
		occlusion += step - here;
	step = getColorAt(vec2(0, -Offset));
	if (step > here)
		occlusion += step - here;
	return occlusion;
}

void main()
{
	const vec3 AmbientColor = vec3(1.1);
	const vec3 DiffuseColor = vec3(0.3);

	vec3 Normal;
	float Offset = 1.0 / uLayerWidth;
	Normal.x = texture(uHeightMap, vTexCoords + vec2(-Offset, 0.0)).r - texture(uHeightMap, vTexCoords + vec2(Offset, 0.0)).r;
	Normal.z = texture(uHeightMap, vTexCoords + vec2(0.0, Offset)).r - texture(uHeightMap, vTexCoords + vec2(0.0, -Offset)).r;
	Normal.y = 4.0 * Offset;
	Normal = normalize(Normal);
	Normal *= normalize(getNormalAt(vec2(0.0)));

	float occlusion = getOcclusion(Offset*8.0)*1.3;
	float occlusionhigh = getHighFrequencyOcclusion(Offset)*1.3;
	float occlusionmid =
		getHighFrequencyOcclusion(Offset*4.0) +
		getHighFrequencyOcclusion(Offset*8.0) +
		getHighFrequencyOcclusion(Offset*16.0) +
		getHighFrequencyOcclusion(Offset*24.0);
	occlusionmid /= 4;


	vec3 Diffuse = DiffuseColor * clamp(dot(Normal, vLight), 0.0, 1.0);
	vec3 Ambient = AmbientColor * ((Normal.x + Normal.y + Normal.z) / 12.0 + 0.6);

	if (uDebugHeight == 0)
	{
		if (uDebugMode == 0)
			gl_FragData[0] = vec4(vec3(
				((1.0 - occlusionmid*0.15) + (1.0 - occlusionhigh*0.15) + (1.0 - occlusion*0.3)) / 3.0
				), 1.0) * texture(uColorMap, vTexCoords);
		else if (uDebugMode == 1)
			gl_FragData[0] = vec4(vec3(1.0 - getHighFrequencyOcclusion(Offset*4.0)*1.3*0.15), 1.0);//texture(uColorMap, vTexCoords);
		else if (uDebugMode == 2)
			gl_FragData[0] = vec4(vec3(1.0 - getHighFrequencyOcclusion(Offset*8.0)*1.3*0.15), 1.0);//texture(uColorMap, vTexCoords) * vec4(vec3(1.0 - occlusion2*0.15), 1.0);
		else if (uDebugMode == 3)
			gl_FragData[0] = vec4(vec3(1.0 - getHighFrequencyOcclusion(Offset*16.0)*1.3*0.15), 1.0);//texture(uColorMap, vTexCoords) * vec4(vec3(1.0 - occlusion*0.3), 1.0);
		else if (uDebugMode == 4)
			gl_FragData[0] = vec4(vec3(1.0 - getHighFrequencyOcclusion(Offset*24.0)*1.3*0.15), 1.0);//texture(uColorMap, vTexCoords) * vec4(vec3(1.0 - occlusion2*0.15), 1.0) * vec4(vec3(1.0 - occlusion*0.3), 1.0);
		else if (uDebugMode == 5)
			gl_FragData[0] = vec4(vec3(1.0 - occlusionmid*0.15), 1.0);
		else if (uDebugMode == 6)
			gl_FragData[0] = vec4(vec3(1.0 - occlusionhigh*0.15), 1.0);
		else if (uDebugMode == 7)
			gl_FragData[0] = vec4(vec3(1.0 - occlusion*0.3), 1.0);
		else if (uDebugMode == 8)
			gl_FragData[0] = texture(uColorMap, vTexCoords);
		else if (uDebugMode == 9)
			gl_FragData[0] = vec4(vec3(
				((1.0 - occlusionmid*0.15) + (1.0 - occlusionhigh*0.15) + (1.0 - occlusion*0.3)) / 3.0
				), 1.0);
		else if (uDebugMode == 10)
			gl_FragData[0] = vec4(texture(uNormalMap, vTexCoords).rgb, 1.0);
		else if (uDebugMode == 11)
			gl_FragData[0] = vec4(Normal * vec3(0.5) + vec3(0.5), 1.0);
		else if (uDebugMode == 12)
			gl_FragData[0] = vec4(Diffuse + Ambient, 1.0);
		else
			gl_FragData[0] = texture(uColorMap, vTexCoords);
	}
	else
		gl_FragData[0] = vec4(vec3(vHeight), 1.0);

	gl_FragData[1] = vec4((vScreenPosition.z / vScreenPosition.w + 1.0) / 2.0, 0.0, 0.0, 1.0);
}
