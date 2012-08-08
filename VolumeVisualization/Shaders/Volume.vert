attribute vec3 aPosition;
attribute vec3 aColor;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

varying vec3 vColor;
varying vec4 vPosition;

void main()
{
    vPosition = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
    gl_Position = vPosition;
    
    vColor = aColor;
}
