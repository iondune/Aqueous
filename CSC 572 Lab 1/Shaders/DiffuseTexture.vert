attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;

varying vec4 vColor;
varying vec3 vLight;
varying vec3 vNormal;
varying vec2 vTexCoord;

void main()
{
    const vec3 LightPosition = vec3(1, 2, 1);

    vec4 vPosition;

    vPosition = uModelMatrix * vec4(aPosition, 1);
    vLight = normalize(LightPosition - vec3(vPosition));
    gl_Position = uProjMatrix * uViewMatrix * vPosition;

    vTexCoord = vec2(aTexCoord.x, aTexCoord.y);

    vLight = normalize(LightPosition - vec3(vPosition));
    vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
