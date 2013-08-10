#version 120
 
varying vec2 texcoord;
uniform sampler2D tex;
 
void main(void) {
    gl_FragColor = vec4(0, 0, 0, texture2D(tex, texcoord).a);
}
