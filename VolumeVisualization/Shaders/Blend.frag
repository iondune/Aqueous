uniform sampler2D scene;
uniform sampler2D bloom;

varying vec2 vTexCoord;

void main()
{
    vec4 SceneColor = texture2D(scene, vTexCoord);
    vec4 BloomColor = texture2D(bloom, vTexCoord);
    
    vec4 Result;
    for (int i = 0; i < 4; ++ i)
      Result[i] = max(SceneColor[i], BloomColor[i]);
      
    gl_FragColor = Result;
}
