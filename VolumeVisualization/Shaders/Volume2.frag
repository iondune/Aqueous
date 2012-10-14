#version 150

in vec3 vColor;
in vec4 vPosition;

out vec4 outFragColor;

uniform sampler3D uVolumeData;
uniform sampler2D uDepthTexture;

uniform mat4 uModelMatrix;
uniform mat4 uInvModelMatrix;
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

vec4 getColorSample(vec3 coords)
{
	vec4 sample = texture(uVolumeData, coords);

	switch (uHighlightMode)
	{
	default:
	case 0:
		sample.a = 0.8;
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

	case 3:
		break;
	}

	return sample;
}

void main()
{
	vec3 BackPosition = vColor;

	// Calculate surface point
	vec3 FrontPosition;
	vec3 CameraPosition = (uInvModelMatrix * vec4(uCameraPosition, 1.0)).xyz;
	
	if (CameraPosition.x >= -0.5 && 
		CameraPosition.y >= -0.5 && 
		CameraPosition.z >= -0.5 && 
		CameraPosition.x <=  0.5 && 
		CameraPosition.y <=  0.5 && 
		CameraPosition.z <=  0.5)
	{
		FrontPosition = CameraPosition + vec3(0.5);
	}
	else
	{
		FrontPosition = CameraPosition + vec3(0.5);
		vec3 InternalVector = FrontPosition - BackPosition;
		for (int i = 0; i < 3; ++ i)
		{
			if (FrontPosition[i] > 1.0 || FrontPosition[i] < 0.0)
			{
				if (InternalVector[i] > 0.0)
					InternalVector *= (1 - BackPosition[i]) / InternalVector[i];
				else
					InternalVector *= BackPosition[i] / -InternalVector[i];
				FrontPosition = BackPosition + InternalVector;
			}
		}
	}

	vec3 Direction = BackPosition - FrontPosition;
	float Length = length(Direction);
	
	Direction = normalize(Direction);
	vec3 DirectionStep = Direction * uStepSize;
	
	vec3 Iterator = FrontPosition;
	
	vec4 ColorAccumulator = vec4(0.0);
	float AlphaAccumulator = 0.0;
	float LengthAccumulator = 0.0;

	float CurrentDepth = texture2D(uDepthTexture, ((vPosition.xy / vPosition.w) + 1.0) / 2.0).r;

	for(int i = 0; i < 1000; i ++)
	{
		// Generate samples
		vec4 ColorSample = getColorSample(Iterator);
		float AlphaSample = ColorSample.a * uStepSize * uAlphaIntensity;
		
		// Accumulate
		ColorAccumulator += (1.0 - AlphaAccumulator) * ColorSample * AlphaSample * 3;
		AlphaAccumulator += AlphaSample;
		LengthAccumulator += length(DirectionStep);
		
		// Advance iterator
		Iterator += DirectionStep;
		
		// Calculate depth
		vec4 ScreenCoords = vec4(Iterator, 1.0);
		ScreenCoords -= vec4(vec3(0.5), 0.0);
		ScreenCoords = uProjMatrix * uViewMatrix * uModelMatrix * ScreenCoords;

		float Depth = ScreenCoords.z / ScreenCoords.w;
		Depth += 1.0;
		Depth /= 2.0;

		// Depth test
		if (Depth > CurrentDepth)
		{
			if (uDebugLevel == 2)
			{
				outFragColor = vec4(0, 0, 0, 1);
				return;
			}
			break;
		}

		// Accumulation test
		if (LengthAccumulator >= Length || AlphaAccumulator > 1.0)
		{
			if (uDebugLevel == 2)
			{
				outFragColor = vec4(1, 1, 1, 1);
				return;
			}
			break;
		}
	}

	if (uDebugLevel == 1)
		outFragColor = vec4(FrontPosition, 1.0);
	else
		outFragColor = ColorAccumulator;
}
