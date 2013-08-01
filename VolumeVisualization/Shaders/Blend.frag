uniform sampler2D scene;
uniform sampler2D ssao;
uniform sampler2D bloom;

varying vec2 vTexCoord;

void main()
{
   vec4 color = texture2D(bloom, vTexCoord);
    vec4 color2 = texture2D(scene, vTexCoord);
    float SSAOconst = pow(texture2D(ssao, vTexCoord).r, 1.5);
//Don't do bloom
   if(color.r == 1.0 && color.g == 0.0 && color.b == 1.0) {
      gl_FragColor = color2*SSAOconst;
   }
//Do bloom (brighter color)
   //else if(color.r > 0.5 && color.g > 0.5 && color.b > 0.5) {
   else if(color.r > color2.r && color.g > color2.g && color.b > color2.b) {
      gl_FragColor = color*SSAOconst;
   }
//Do bloom (darker color)
   else {
      gl_FragColor = color2*SSAOconst;
   }
}
