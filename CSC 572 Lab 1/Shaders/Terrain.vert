#extension GL_EXT_gpu_shader4 : enable

attribute vec2 aPosition;
attribute vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;


uniform float uLayerWidth;

uniform sampler2D uHeightMap;
uniform vec2 uDataOffset;

uniform sampler2D uCourseMap;
uniform vec2 uCourseOffset;

uniform int uUseCourse;

uniform vec2 uActiveRegionLL;
uniform vec2 uActiveRegionUR;

uniform vec3 uLightPosition;

varying vec3 vLight;
varying vec3 vNormal;
varying vec2 vTexCoords;
varying vec4 vColor;

void main()
{
    vTexCoords = (aPosition.xy + uDataOffset) / (uLayerWidth + 1.0);
    vec2 HeightmapCoords = vTexCoords + 
        // half pixel adjustment for floating-point issues on some cards
        (1.0 / uLayerWidth / 2.0);
    
    vec4 vPosition = vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords).r, aPosition.y, 1);
    vColor = vec4(vPosition.y, vPosition.y, vPosition.y, 1.0);
    
    if (uUseCourse != 0)
    {
        vec2 ActiveCoords = (aPosition.xy - uActiveRegionLL) / (uActiveRegionUR - uActiveRegionLL);
        
        bool doInterp = false;
        vec2 ActiveInterp = vec2(0.0, 0.0);
        
        if (ActiveCoords.x < 0.1)
        {
            doInterp = true;
            ActiveInterp.x = 1.0 - ActiveCoords.x / 0.1;
        }
        else if (ActiveCoords.x > 0.9)
        {
            doInterp = true;
            ActiveInterp.x = (ActiveCoords.x - 0.9) / 0.1;
        }
        
        if (ActiveCoords.y < 0.1)
        {
            doInterp = true;
            ActiveInterp.y = 1.0 - ActiveCoords.y / 0.1;
        }
        else if (ActiveCoords.y > 0.9)
        {
            doInterp = true;
            ActiveInterp.y = (ActiveCoords.y - 0.9) / 0.1;
        }
        
        if (doInterp)
        {
            float UseInterp = max(0.0, min(1.0, max(ActiveInterp.x, ActiveInterp.y)));
            
            
            //vec2 vTexCourse = (aPosition.xy + uCourseOffset) / (uLayerWidth + 1.0);
            //vTexCourse -= 0.5;
            //vTexCourse *= 0.5;
            //vTexCourse += 0.5;
            //vec2 CoursemapCoords = vTexCourse + 
                // half pixel adjustment for floating-point issues on some cards
                //(1.0 / uLayerWidth / 2.0);
            
            //vec4 vCourse = uModelMatrix * vec4(aPosition.x, texture2D(uCourseMap, CoursemapCoords).r, aPosition.y, 1);
            
            vec4 vCourse = vec4(0.0, 0.0, 0.0, 0.0);

            if (ActiveCoords.x < -0.01 || ActiveCoords.y < -0.01 || ActiveCoords.y > 1.01 || ActiveCoords.x > 1.01)
            {
                //vPosition.y += 1000;
                //vCourse.y += 1000;
            }
            
            if ((int(aPosition.x) % 2) != 0 && (int(aPosition.y) % 2) != 0)
            {
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords + vec2((-1.0 / uLayerWidth), (-1.0 / uLayerWidth))).r, aPosition.y, 1) * 0.25;
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords + vec2((-1.0 / uLayerWidth), ( 1.0 / uLayerWidth))).r, aPosition.y, 1) * 0.25;
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords + vec2(( 1.0 / uLayerWidth), (-1.0 / uLayerWidth))).r, aPosition.y, 1) * 0.25;
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords + vec2(( 1.0 / uLayerWidth), ( 1.0 / uLayerWidth))).r, aPosition.y, 1) * 0.25;
            }
            else if ((int(aPosition.x) % 2) != 0)
            {
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords + vec2((-1.0 / uLayerWidth), 0)).r, aPosition.y, 1) * 0.5;
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords + vec2(( 1.0 / uLayerWidth), 0)).r, aPosition.y, 1) * 0.5;
            }
            else if ((int(aPosition.y) % 2) != 0)
            {
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords + vec2(0, (-1.0 / uLayerWidth))).r, aPosition.y, 1) * 0.5;
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords + vec2(0, (1.0 / uLayerWidth))).r, aPosition.y, 1) * 0.5;
            }
            else
            {
                vCourse += vec4(aPosition.x, texture2D(uHeightMap, HeightmapCoords).r, aPosition.y, 1);
            }

            vCourse *= UseInterp;
            //vCourse.x = vPosition.x;
            //vCourse.z = vPosition.z;
            vPosition *= 1.0 - UseInterp;
            
            vPosition += vCourse;
            //vPosition.y += 5.0 * UseInterp;
        }
    }

    vPosition.y *= 45.0;
    
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vPosition;
    
    vec3 LightPosition = uLightPosition;
    
    vLight = normalize(LightPosition - vec3(uModelMatrix * vPosition));
    
    vNormal.x = texture2D(uHeightMap, HeightmapCoords + vec2((-1.0 / uLayerWidth), 0.0)).r - texture2D(uHeightMap, HeightmapCoords + vec2((1.0 / uLayerWidth), 0.0)).r;
    vNormal.z = texture2D(uHeightMap, HeightmapCoords + vec2(0.0, (-1.0 / uLayerWidth))).r - texture2D(uHeightMap, HeightmapCoords + vec2(0.0, (1.0 / uLayerWidth))).r;
    vNormal.y = 2.0 / uLayerWidth + 2.0 / uLayerWidth;
    vNormal = normalize(vNormal);
}
