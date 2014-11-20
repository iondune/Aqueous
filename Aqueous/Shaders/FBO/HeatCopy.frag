uniform sampler2D uTexColor;
uniform sampler2D uHeatOffset;

uniform float uTimer;

varying vec2 vTexCoord;

void main()
{
    float OffsetStrength = 0.03;
    vec2 InternalTex = vTexCoord + vec2(uTimer * 0.2, -uTimer);
    vec2 HeatOffset = (texture2D(uHeatOffset, InternalTex).rg - 0.5) * OffsetStrength;
    //HeatOffset.y = HeatOffset.x;
    //HeatOffset = vec2(0.03, 0.03);
    vec4 OffsetColor = texture2D(uTexColor, vTexCoord + HeatOffset);
    vec4 MainColor = texture2D(uTexColor, vTexCoord);
    
    //OffsetColor = vec4(HeatOffset, 0, 0);
    /*
    OffsetColor.r = min(OffsetColor.r, 1.0);
    OffsetColor.g = min(OffsetColor.g, 1.0);
    OffsetColor.b = min(OffsetColor.b, 1.0);
    OffsetColor.r = max(OffsetColor.r, 0.0);
    OffsetColor.g = max(OffsetColor.g, 0.0);
    OffsetColor.b = max(OffsetColor.b, 0.0);
    */
    gl_FragColor = OffsetColor;
    
    /*
    float Interp = 0.4;
    vec4 BlurColor = vec4(max(MainColor.r, OffsetColor.r), max(MainColor.g, OffsetColor.g), max(MainColor.b, OffsetColor.b), 1.0);
    gl_FragColor = MainColor * Interp + BlurColor * (1.0 - Interp);
    */
}
