attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;
attribute vec3 aTangent;
attribute vec3 aBinormal;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uInvModelMatrix;

uniform vec3 uLightPosition;

varying vec4 vColor;
varying vec3 vLight;
varying vec2 vTexCoord;

void main()
{
    // Send texture coords to frag shader
    vTexCoord = vec2(aTexCoord.x, aTexCoord.y);
    
    // Compute TBN Matrix
    mat4 TBN = mat4(
        vec4(aTangent.x, aBinormal.x, aNormal.x, 0),
        vec4(aTangent.y, aBinormal.y, aNormal.y, 0),
        vec4(aTangent.z, aBinormal.z, aNormal.z, 0),
        vec4(0, 0, 0, 1)
    );
    
    // Get tangent space light position
    vec4 LightPosition = vec4(uLightPosition, 1.0);
    LightPosition = uInvModelMatrix * LightPosition;
    LightPosition = TBN * LightPosition;
    
    // Get light vector
    vec4 Position = uModelMatrix * vec4(aPosition, 1);
    vLight = normalize(LightPosition.xyz - vec3(Position));
    
    // Send vertex position
    gl_Position = uProjMatrix * uViewMatrix * Position;
}
