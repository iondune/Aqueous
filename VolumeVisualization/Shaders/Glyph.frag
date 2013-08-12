uniform vec3 uColor;

varying vec3 vLight;
varying vec3 vNormal;

void main()
{
    const vec3 AmbientColor = vec3(0.6, 0.6, 0.6);
    const vec3 DiffuseColor = vec3(0.4, 0.4, 0.4);

    vec3 Diffuse = DiffuseColor * clamp(dot(normalize(vNormal), normalize(vLight)), 0.0, 1.0);

    gl_FragColor = vec4(uColor.rgb * (Diffuse + AmbientColor), 1.0);
}
