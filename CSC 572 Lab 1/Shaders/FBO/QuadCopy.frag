uniform sampler2D uTexColor;

varying vec2 vTexCoord;

void main()
{
    gl_FragColor = texture2D(uTexColor, vec2(vTexCoord));
}
