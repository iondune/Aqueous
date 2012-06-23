attribute vec2 aPosition;

varying vec2 vTexCoord;

void main()
{
    gl_Position = vec4(aPosition, 0, 1);
    vTexCoord = (aPosition + vec2(1.0)) / 2.0;
}
