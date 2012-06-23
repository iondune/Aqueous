uniform sampler2D uTexColor;
uniform float BlurSize;
uniform float DimAmount;

varying vec2 vTexCoord;

void main()
{
    float blurSize = 0.0025*BlurSize;
    vec3 color = (texture2D(uTexColor, vec2(vTexCoord.x - 4.0*blurSize, vTexCoord.y))*0.05 +
                  texture2D(uTexColor, vec2(vTexCoord.x - 3.0*blurSize, vTexCoord.y))*0.1 +
                  texture2D(uTexColor, vec2(vTexCoord.x - 2.0*blurSize, vTexCoord.y))*0.2 +
                  texture2D(uTexColor, vec2(vTexCoord.x - 1.0*blurSize, vTexCoord.y))*0.4 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y))*1.0 +
                  texture2D(uTexColor, vec2(vTexCoord.x + 1.0*blurSize, vTexCoord.y))*0.4 +
                  texture2D(uTexColor, vec2(vTexCoord.x + 2.0*blurSize, vTexCoord.y))*0.2 +
                  texture2D(uTexColor, vec2(vTexCoord.x + 3.0*blurSize, vTexCoord.y))*0.1 +
                  texture2D(uTexColor, vec2(vTexCoord.x + 4.0*blurSize, vTexCoord.y))*0.05
                 ).xyz / 2.5;
    gl_FragColor = vec4(color, 1.0)*DimAmount;
}
