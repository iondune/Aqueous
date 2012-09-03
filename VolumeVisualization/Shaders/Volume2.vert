#version 150

in vec3 aPosition;
in vec3 aColor;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

out vec3 vColor;
out vec4 vPosition;

void main()
{
    vPosition = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition.xyz, 1);
    gl_Position = vPosition;
    
    vColor = aColor;
}
