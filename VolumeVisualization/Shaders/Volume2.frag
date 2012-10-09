#version 150

in vec3 vColor;
in vec4 vPosition;

out vec4 outFragColor;

uniform sampler3D uVolumeData;
uniform sampler2D uDepthTexture;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uCameraPosition;

uniform float uAlphaIntensity;
uniform float uStepSize;
uniform int   uHighlightMode;
uniform vec3  uSliceAxis;
uniform float uLocalRange;
uniform float uMinimumAlpha;
uniform float uEmphasisLocation;

uniform int uDebugLevel;

float enter;
float Exit;


bool rayAABBIntersect1D(float start, float dir, float Min, float Max)
{
    // ray parallel to direction
    if(abs(dir) < 1.0E-6) 
        return (start >= Min && start <= Max);

    // intersection params
    float t0, t1;
    t0 = (Min - start) / dir;
    t1 = (Max - start) / dir;

    // sort intersections
    if(t0 > t1)
    {
    	float temp = t0;
    	t0 = t1;
    	t1 = temp;
    }

    // check if intervals are disjoint
    if(t0 > Exit || t1 < enter) 
        return false;

    // reduce interval
    if(t0 > enter) enter = t0;
    if(t1 < Exit) Exit = t1;
    return true;
}

vec3 penter = vec3(0.0);
vec3 pexit;

bool rayAABBIntersect(vec3 start, vec3 dir, vec3 Min, vec3 Max)
{
    enter = 0.0;
    Exit = 1.0;

    if(!rayAABBIntersect1D(start.x, dir.x, Min.x, Max.x))
        return false;

    if(!rayAABBIntersect1D(start.y, dir.y, Min.y, Max.y))
        return false;
    
    if(!rayAABBIntersect1D(start.z, dir.z, Min.z, Max.z))
        return false;

    penter = start + dir * enter;
    pexit  = start + dir * Exit;
    return true;
}

vec4 getColorSample(vec3 coords)
{
	vec4 sample = texture(uVolumeData, coords);

	switch (uHighlightMode)
	{
	default:
	case 0:
		sample.a = 0.8;
		break;

	case 3:
		break;

	case 1:
		vec3 LocalVector = coords - 0.5;
		vec3 PlanarVector = normalize(uSliceAxis);

		float Expansion = 1.0;
		vec3 MVec = abs(PlanarVector);
		if (MVec.x > MVec.y && MVec.x > MVec.z)
		{
			Expansion = 1.0 / MVec.x;
		}
		else if (MVec.y > MVec.x && MVec.y > MVec.z)
		{
			Expansion = 1.0 / MVec.y;
		}
		else if (MVec.z > MVec.x && MVec.z > MVec.y)
		{
			Expansion = 1.0 / MVec.z;
		}

		float Distance = abs(dot(LocalVector / Expansion, PlanarVector) + 0.5 - uEmphasisLocation);

		if (Distance < uLocalRange / 2.0)
		{
			float Ratio = 1.0 - Distance / (uLocalRange / 2.0);
			sample.a = Ratio * (1.0 - uMinimumAlpha) + uMinimumAlpha;
		}
		else
		{
			sample.a = uMinimumAlpha;
		}
		break;

	case 2:
		float Height = sample.a;

		if (abs(Height - uEmphasisLocation) < uLocalRange / 2.0)
		{
			float Ratio = 1.0 - abs(Height - uEmphasisLocation) / (uLocalRange / 2.0);
			sample.a = Ratio * (1.0 - uMinimumAlpha) + uMinimumAlpha;
		}
		else
		{
			sample.a = uMinimumAlpha;
		}
		break;
	}

	return sample;
}

void main()
{
	vec3 BackPosition = vColor;

	vec3 FrontPosition;

	vec3 CameraPosition = (inverse(uModelMatrix) * vec4(uCameraPosition, 1.0)).xyz;

	// Calculate surface point
	if (CameraPosition.x >= -0.5 && 
		CameraPosition.y >= -0.5 && 
		CameraPosition.z >= -0.5 && 
		CameraPosition.x <=  0.5 && 
		CameraPosition.y <=  0.5 && 
		CameraPosition.z <=  0.5)
	{
		FrontPosition = CameraPosition + vec3(0.5);
		/*if (uDebugLevel != 0)
		{
			outFragColor = vec4(1, 0, 0, 1);
			return;
		}*/
	}
	else
	{
		//FrontPosition.x = BackPosition.x + 1.0 / (CameraPosition.y - BackPosition.y) * (CameraPosition.x - BackPosition.x);
		//FrontPosition.y = BackPosition.y + 1.0 / (CameraPosition.z - BackPosition.z) * (CameraPosition.y - BackPosition.y);
		//FrontPosition.z = BackPosition.z + 1.0 / (CameraPosition.x - BackPosition.x) * (CameraPosition.z - BackPosition.z);
		if (uDebugLevel == 0)
		{
			FrontPosition = CameraPosition + vec3(0.5);
			vec3 InternalVector = FrontPosition - BackPosition;
			float MaxLength = max(max(abs(InternalVector.x), abs(InternalVector.y)), abs(InternalVector.z));
			InternalVector /= MaxLength;
			FrontPosition = InternalVector + BackPosition;
		}
		else
		{
			if (rayAABBIntersect(CameraPosition.xyz, (BackPosition - vec3(0.5)) - CameraPosition.xyz, vec3(-0.5), vec3(0.5)))
			{
				FrontPosition = penter;
				/*if (uDebugLevel != 0)
				{
					outFragColor = vec4(0, 1, 0, 1);
					return;
				}*/
			}
			else
			{
				FrontPosition = CameraPosition.xyz;
				/*if (uDebugLevel != 0)
				{
					outFragColor = vec4(0, 0, 1, 1);
					return;
				}*/
			}
			FrontPosition += vec3(0.5);
		}
	}


	vec3 start = FrontPosition;
	vec3 dir = BackPosition - FrontPosition;

	float len = length(dir.xyz); // the length from front to back is calculated and used to terminate the ray
	vec3 norm_dir = normalize(dir);
	float delta = uStepSize;
	vec3 delta_dir = norm_dir * delta;
	float delta_dir_len = length(delta_dir);
	vec3 vec = start;
	vec4 col_acc = vec4(0,0,0,0);
	float alpha_acc = 0;
	float length_acc = 0;

	float CurrentDepth = texture2D(uDepthTexture, ((vPosition.xy / vPosition.w) + 1.0) / 2.0).r;

	for(int i = 0; i < 1000; i ++)
	{
		//vec4 color_sample = vec4(vec, 0.5);
		

		vec4 color_sample = getColorSample(vec);
		float alpha_sample = color_sample.a * uStepSize * uAlphaIntensity;
		col_acc   += (1.0 - alpha_acc) * color_sample * alpha_sample * 3;
		//col_acc   += color_sample;
		alpha_acc += alpha_sample;
		vec += delta_dir;
		length_acc += delta_dir_len;


		vec4 ScreenCoords = vec4(vec, 1.0);
		ScreenCoords -= vec4(0.5, 0.5, 0.5, 0.0);
		ScreenCoords = uProjMatrix * uViewMatrix * uModelMatrix * ScreenCoords;

		float Depth = ScreenCoords.z / ScreenCoords.w;
		Depth += 1.0;
		Depth /= 2.0;

		if (Depth > CurrentDepth)
		{
			//if (Debug == 1)
			{
				//gl_FragColor = vec4(0, 0, 0, 1);
			}
			break;
		}

		if (length_acc >= len || alpha_acc > 1.0)
		{
			//if (Debug == 1)
			{
				//gl_FragColor = vec4(1, 1, 1, 1);
			}
			break; // terminate if opacity > 1 or the ray is outside the volume
		}

		/*if (i == 50 && Debug != 0)
		{
			gl_FragColor = vec4(1, 0, 1, 1);
			return;
		}*/
	}


    outFragColor = vec4(FrontPosition, 1.0) + 0.0001* col_acc;
}
