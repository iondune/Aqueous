#version 120

uniform mat4 mvp;

attribute vec2 position;

attribute vec2 texcoord;
varying vec2 frag_texcoord;

attribute vec4 color;
varying vec4 frag_color;

void main() {
    gl_Position = mvp * vec4(position, 0.0, 1.0);
    frag_texcoord = texcoord;
    frag_color = color;
}
