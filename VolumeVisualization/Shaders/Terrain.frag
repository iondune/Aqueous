#version 130

in vec3 vLight;
in vec2 vTexCoords;
in vec4 vScreenPosition;

uniform sampler2D uColorMap;
uniform sampler2D uHeightMap;

uniform int uDebugHeight;
uniform float uLayerWidth;

out vec4 gl_FragData[2];


float getHeightAt(vec2 Offset)
{
	return texture(uHeightMap, vTexCoords + Offset);
}

void main()
{
	const vec3 AmbientColor = vec3(0.2, 0.2, 0.2);
	const vec3 DiffuseColor = vec3(0.8, 0.8, 0.8);

	vec3 vNormal;
	float Offset = 8.0 / uLayerWidth;
	//vNormal.x = texture(uHeightMap, vTexCoords + vec2((-Offset), 0.0)).r - texture(uHeightMap, vTexCoords + vec2((Offset), 0.0)).r;
	//vNormal.z = texture(uHeightMap, vTexCoords + vec2(0.0, (-Offset))).r - texture(uHeightMap, vTexCoords + vec2(0.0, (Offset))).r;
	vNormal.y = 4.0 * Offset;
	//vNormal = normalize(vNormal);

	vec3 vDiffuse = DiffuseColor * clamp(dot(vNormal, vLight), 0.0, 1.0);

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

	if (uDebugHeight == 0)
		gl_FragData[0] = vec4(vec3(0.85 - occlusion*0.7), 1.0)
			+ 0.001 * vec4(vDiffuse + AmbientColor, 1) + 0.001 * texture(uColorMap, vTexCoords);
	else
		gl_FragData[0] = vec4(vDiffuse + AmbientColor, 1) * texture(uHeightMap, vTexCoords);

	gl_FragData[1] = vec4((vScreenPosition.z / vScreenPosition.w + 1.0) / 2.0, 0.0, 0.0, 1.0);
}
