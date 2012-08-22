uniform sampler2D uTexColor;
vec2 uOffset = vec2(1.0 / 1280.0, 1.0 / 720.0);

varying vec2 vTexCoord;

float weight[5] = {0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162};

void main()
{
    vec3 color = vec3(0);
    for (int i = 0; i < 5; ++ i)
      color += texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y - i * uOffset.y))*weight[i];
    for (int i = 1; i < 5; ++ i)
      color += texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y + i * uOffset.y))*weight[i];
    gl_FragColor = vec4(color, 1.0);
}
