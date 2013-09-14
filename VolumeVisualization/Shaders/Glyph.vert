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

varying vec3 vLight;
varying vec3 vNormal;

void main()
{
	vec4 Position = uModelMatrix * vec4(uPosition + aPosition * vec3(uGlyphSize) / uScale, 1.0);
	gl_Position = uProjMatrix * uViewMatrix * Position;
	gl_ClipDistance[0] = dot(vec4(0,1,0,0), Position);

	vLight = uLightPosition - vec3(Position);
	vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
