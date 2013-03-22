varying vec4 vColor;
varying vec3 vLight;
varying vec4 vScreenPosition;

void main()
{
    const vec3 AmbientColor = vec3(0.6, 0.6, 0.6);
    const vec3 DiffuseColor = vec3(0.4, 0.4, 0.4);
    
    vec3 vDiffuse = DiffuseColor;
    
    gl_FragColor = vec4(vColor.rgb * (vDiffuse + AmbientColor), 1.0);
}
