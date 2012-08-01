varying vec3 vLight;
varying vec3 vNormal;
varying vec2 vTexCoords;

uniform sampler2D uColorMap;
varying vec4 vColor;

void main()
{
    const vec3 AmbientColor = vec3(0.8, 0.8, 0.8);
    const vec3 DiffuseColor = vec3(0.8, 0.8, 0.8);
    
    vec3 vDiffuse = DiffuseColor * clamp(dot(vNormal, vLight), 0.0, 1.0);
    
    
    
    if (1)
   		gl_FragColor = vec4(vDiffuse + AmbientColor, 1) /* vColor + 0.001 */* texture2D(uColorMap, vTexCoords);
    else
   		gl_FragColor = vec4(vDiffuse + AmbientColor, 1) * vColor + 0.001 * texture2D(uColorMap, vTexCoords);
}
