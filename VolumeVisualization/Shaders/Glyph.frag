uniform vec3 uColor;

varying vec4 vColor;
varying vec3 vLight;
varying vec3 vNormal;
varying vec4 vScreenPosition;

void main()
{
    const vec3 AmbientColor = vec3(0.6, 0.6, 0.6);
    const vec3 DiffuseColor = vec3(0.4, 0.4, 0.4);
    
    float Distance = length(vLight);

    vec3 vDiffuse = DiffuseColor * clamp(dot(normalize(vNormal), normalize(vLight)), 0.0, 1.0);

    gl_FragColor = vec4(uColor.rgb * (vDiffuse + AmbientColor), 1.0);
}
