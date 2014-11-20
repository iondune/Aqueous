attribute vec3 aPosition;
attribute vec3 aColor;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

varying vec4 vColor;
varying vec3 vLight;
varying vec4 vScreenPosition;

void main()
{
	vec4 Position = uModelMatrix * vec4(aPosition, 1.0);
	vScreenPosition = uProjMatrix * uViewMatrix * Position;
	gl_ClipDistance[0] = dot(vec4(0, 1, 0, 0), Position);
	gl_Position = vScreenPosition;

	vColor = vec4(aColor, 1.0);
}
