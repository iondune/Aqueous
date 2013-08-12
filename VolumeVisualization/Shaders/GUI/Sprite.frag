#version 120

varying vec2 frag_texcoord;
varying vec4 frag_color;

uniform sampler2D tex;

void main() {
    gl_FragColor = frag_color * texture2D(tex, frag_texcoord);
}
