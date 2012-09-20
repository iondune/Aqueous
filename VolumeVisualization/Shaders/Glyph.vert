attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;

uniform vec3 uLightPosition;

varying vec4 vColor;
varying vec3 vLight;
varying vec3 vNormal;
varying vec4 vScreenPosition;

void main()
{
    vec4 vPosition;

    vPosition = uModelMatrix * vec4(aPosition, 1);
    vScreenPosition = uProjMatrix * uViewMatrix * vPosition;
    gl_Position = vScreenPosition;

    vLight = (uLightPosition - vec3(vPosition));
    vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
