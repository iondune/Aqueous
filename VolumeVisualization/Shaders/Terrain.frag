#version 130

in vec3 vLight;
in vec2 vTexCoords;
in vec4 vScreenPosition;

uniform sampler2D uColorMap;
uniform sampler2D uHeightMap;

uniform int uDebugHeight;
uniform float uLayerWidth;

out vec4 gl_FragData[2];


void main()
{
	const vec3 AmbientColor = vec3(0.8, 0.8, 0.8);
	const vec3 DiffuseColor = vec3(0.8, 0.8, 0.8);

	vec3 vNormal;
	float Offset = 2.0 / uLayerWidth;
	vNormal.x = texture(uHeightMap, vTexCoords + vec2((-Offset), 0.0)).r - texture(uHeightMap, vTexCoords + vec2((1.0 / uLayerWidth), 0.0)).r;
	vNormal.z = texture(uHeightMap, vTexCoords + vec2(0.0, (-Offset))).r - texture(uHeightMap, vTexCoords + vec2(0.0, (Offset))).r;
	vNormal.y = 4.0 * Offset;
	vNormal = normalize(vNormal);

	vec3 vDiffuse = DiffuseColor * clamp(dot(vNormal, vLight), 0.0, 1.0);

	if (uDebugHeight == 0)
		gl_FragData[0] = vec4(vDiffuse + AmbientColor, 1) * texture(uColorMap, vTexCoords);
	else
		gl_FragData[0] = vec4(vDiffuse + AmbientColor, 1) * texture(uHeightMap, vTexCoords);

	gl_FragData[1] = vec4((vScreenPosition.z / vScreenPosition.w + 1.0) / 2.0, 0.0, 0.0, 1.0);
}
