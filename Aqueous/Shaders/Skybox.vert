
attribute vec3 aPosition;
attribute vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uNormalMatrix;

varying vec2 vTexCoord;


void main()
{
    vec4 Position = uModelMatrix * vec4(aPosition, 1);
    vec4 ScreenPosition = uProjMatrix * uViewMatrix * Position;
    gl_Position = ScreenPosition;

    vTexCoord = aTexCoord;
}
