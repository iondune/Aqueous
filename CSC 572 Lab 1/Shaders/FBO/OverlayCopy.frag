uniform sampler2D uTexColor;
uniform sampler2D uHeatOffset;

uniform vec3 uOverlayColor;

varying vec2 vTexCoord;

void main()
{
    vec4 MainColor = texture2D(uTexColor, vTexCoord);
    gl_FragColor = vec4(uOverlayColor, 1.0) * MainColor;
}
