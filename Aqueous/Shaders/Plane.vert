
attribute vec2 aPosition;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

void main()
{
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0);
}
