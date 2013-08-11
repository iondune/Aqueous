attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;
uniform float uGlyphSize;
uniform vec3 uScale;
uniform vec3 uPosition;

uniform vec3 uLightPosition;

varying vec4 vColor;
varying vec3 vLight;
varying vec3 vNormal;
varying vec4 vScreenPosition;

void main()
{
    vec4 vPosition;

    vPosition = uModelMatrix * vec4((uPosition - vec3(0.5)) + aPosition * vec3(uGlyphSize) / uScale, 1.0);
    vScreenPosition = uProjMatrix * uViewMatrix * vPosition;
    gl_Position = vScreenPosition;

    vLight = uLightPosition - vec3(vPosition);
    vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
