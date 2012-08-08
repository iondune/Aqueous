attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

varying vec3 vColor;

void main()
{
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1);

    vColor = abs(aNormal.xyz);
}
