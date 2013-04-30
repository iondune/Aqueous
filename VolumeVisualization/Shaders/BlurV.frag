#version 120

uniform sampler2D uTexColor;
vec2 uOffset = vec2(1.0 / 1800.0, 1.0 / 900.0); // Pixel offset

varying vec2 vTexCoord;

const float weight[8] = float[](0.2094726563, 0.1832885742, 0.1221923828, 0.0610961914, 0.0222167969, 0.0055541992, 0.0008544922, 0.00006103515625);

void main()
{
    vec3 color = vec3(0);
    for (int i = 0; i <= 7; ++ i)
      color += texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y - float(i) * uOffset.y)).rgb*weight[i];
    for (int i = 1; i <= 7; ++ i)
      color += texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y + float(i) * uOffset.y)).rgb*weight[i];
    gl_FragColor = vec4(color, 1.0);
}
