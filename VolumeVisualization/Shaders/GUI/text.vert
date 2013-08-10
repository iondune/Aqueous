#version 120

attribute vec4 coord;
uniform vec2 screenScaling;
uniform vec2 pos;
varying vec2 texcoord;

void main(void) {
    gl_Position = vec4(((coord.xy + pos) * screenScaling) - vec2(1, 1), 0, 1);
    texcoord = coord.zw;
}
