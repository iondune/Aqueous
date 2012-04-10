varying vec4 vColor;
varying vec3 vLight;
varying vec3 vNormal;

void main()
{
    const vec3 AmbientColor = vec3(0.2, 0.2, 0.2);
    const vec3 DiffuseColor = vec3(0.8, 0.8, 0.8);
    
    float Distance = length(vLight);

    float Radius = 10.0;
    vec3 vDiffuse = Distance > Radius ? vec3(0) : DiffuseColor * clamp(dot(normalize(vNormal), normalize(vLight)), 0.0, 1.0) * (1.0 - Distance / Radius);

    gl_FragColor = vec4(vDiffuse + AmbientColor, 1);
}
