varying vec3 vLight;
varying vec2 vTexCoords;

uniform sampler2D uColorMap;
varying vec4 vColor;

uniform int uDebugHeight;

uniform float uLayerWidth;

varying vec2 HeightmapCoords;
uniform sampler2D uHeightMap;

void main()
{
    const vec3 AmbientColor = vec3(0.8, 0.8, 0.8);
    const vec3 DiffuseColor = vec3(0.8, 0.8, 0.8);
    
    vec3 vNormal;
    float Offset = 2.0 / uLayerWidth;
    vNormal.x = /*textureOffset(uHeightMap, HeightmapCoords, ivec2(-1, 0)).r - textureOffset(uHeightMap, HeightmapCoords, ivec2(1, 0)).r;/*/
      texture2D(uHeightMap, HeightmapCoords + vec2((-Offset), 0.0)).r - texture2D(uHeightMap, HeightmapCoords + vec2((1.0 / uLayerWidth), 0.0)).r;
    vNormal.z = /*textureOffset(uHeightMap, HeightmapCoords, ivec2(0, -1)).r - textureOffset(uHeightMap, HeightmapCoords, ivec2(0, 1)).r;/*/
      texture2D(uHeightMap, HeightmapCoords + vec2(0.0, (-Offset))).r - texture2D(uHeightMap, HeightmapCoords + vec2(0.0, (Offset))).r;
    vNormal.y = 4.0 * Offset;
    vNormal = normalize(vNormal);
    
    vec3 vDiffuse = DiffuseColor * clamp(dot(vNormal, vLight), 0.0, 1.0);
    
    
    
    if (! uDebugHeight)
   		gl_FragColor = vec4(vDiffuse + AmbientColor, 1) /* vColor + 0.001 */* texture2D(uColorMap, vTexCoords);
    else
   		gl_FragColor = vec4(vDiffuse + AmbientColor, 1) * vColor + 0.001 * texture2D(uColorMap, vTexCoords);
}
