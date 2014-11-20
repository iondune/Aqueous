attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vLight;
varying vec3 vNormal;
varying vec2 vTexCoord;





varying vec4 vScreenPosition;

void main()
{
    const vec3 LightPosition = vec3(1, 2, 1);

    vec4 vPosition;

    vPosition = uModelMatrix * vec4(aPosition, 1);



    vLight = normalize(LightPosition - vec3(vPosition));



    vScreenPosition = uProjMatrix * uViewMatrix * vPosition;

    gl_Position = vScreenPosition;

    vTexCoord = aTexCoord;

    vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
