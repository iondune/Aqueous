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
uniform vec3 uLightPosition;

uniform float uAlphaIntensity;
uniform float uStepSize;
uniform int   uHighlightMode;
uniform vec3  uSliceAxis;
uniform float uLocalRange;
uniform float uMinimumAlpha;
uniform float uEmphasisLocation;

uniform int uDebugLevel;
uniform int uUseShading;

vec4 GetColorSample(vec3 coords)
{
	vec4 sample = texture(uVolumeData, coords);

	switch (uHighlightMode)
	{
	default:
	case 0: // Constant-alpha mode
		sample.a = 0.8;
		break;

	case 3: // Sample alpha mode
		break;

	case 1: // Planar-slice mode
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

	case 2: // Isosurface mode
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

float GetValue(vec3 Location)
{
	//vec3 Color = texture(uVolumeData, Location).rgb;
	//return (Color.r + Color.g + Color.b) / 3.0;
	return GetColorSample(Location).a;
}

vec3 GetGradient(vec3 Location)
{
	const float Epsilon = 0.02;
	return normalize(vec3(
		GetValue(Location + vec3(Epsilon, 0, 0)) - GetValue(Location - vec3(Epsilon, 0, 0)),
		GetValue(Location + vec3(0, Epsilon, 0)) - GetValue(Location - vec3(0, Epsilon, 0)),
		GetValue(Location + vec3(0, 0, Epsilon)) - GetValue(Location - vec3(0, 0, Epsilon))));
}

bool Equals(float a, float b)
{
	const float epsilon = 0.0001;
	return (a + epsilon >= b) && (a - epsilon <= b);
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
	
	const int IterationMax = 1000;

	int i;
	for(i = 0; i < IterationMax; i ++)
	{
		// Calculate depth
		float Depth = 0;
		vec4 WorldCoords = uModelMatrix * vec4(Iterator - vec3(0.5), 1.0);
		vec4 ScreenCoords = uProjMatrix * uViewMatrix * WorldCoords;

		if (! Equals(ScreenCoords.w, 0))
		{
			
			Depth = (ScreenCoords.z / ScreenCoords.w + 1.0) / 2.0;
		}

		if (uDebugLevel == 5)
		{
			outFragColor = vec4(Depth, 0, 0, 1);
			return;
		}

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

		// Generate samples
		vec4 ColorSample = GetColorSample(Iterator);
		float AlphaSample = ColorSample.a * uStepSize * uAlphaIntensity;
		
		// Accumulate
		vec3 Normal = GetGradient(Iterator);
		vec3 Light = normalize(uLightPosition - WorldCoords.xyz);

		if (uDebugLevel == 6)
			ColorAccumulator += (1.0 - AlphaAccumulator / uAlphaIntensity) * AlphaSample * 3 * vec4(Normal / 2.0 + vec3(0.5), ColorSample.a);
		else if (uUseShading != 0)
			ColorAccumulator += (1.0 - AlphaAccumulator / uAlphaIntensity) * AlphaSample * 3 * vec4(ColorSample.rgb * (clamp(dot(Normal, Light), 0.0, 1.0) + 0.5), ColorSample.a);
		else
			ColorAccumulator += (1.0 - AlphaAccumulator / uAlphaIntensity) * AlphaSample * 3 * ColorSample;

		AlphaAccumulator += AlphaSample;
		LengthAccumulator += length(DirectionStep);
		
		// Advance iterator
		Iterator += DirectionStep;

		// Length test
		if (LengthAccumulator >= Length)
		{
			if (uDebugLevel == 2)
			{
				outFragColor = vec4(0, float(i) / float(IterationMax), 1, 1);
				return;
			}
			break;
		}
		
		// Accumulation test
		if (AlphaAccumulator > uAlphaIntensity)
		{
			if (uDebugLevel == 3)
			{
				outFragColor = vec4(1, 0, float(i) / float(IterationMax), 1);
				return;
			}
			break;
		}
		
		if (uDebugLevel == 4 && IterationMax == i + 1)
		{
			outFragColor = vec4(0, 0.1, 0.1, 1);
			return;
		}
	}
	
	if (uDebugLevel == 4)
	{
		outFragColor = vec4(1, 0.5, float(i) / float(IterationMax), 1);
		return;
	}
	
	ColorAccumulator.r = clamp(ColorAccumulator.r, 0.0, 1.0);
	ColorAccumulator.g = clamp(ColorAccumulator.g, 0.0, 1.0);
	ColorAccumulator.b = clamp(ColorAccumulator.b, 0.0, 1.0);
	ColorAccumulator.a = clamp(ColorAccumulator.a, 0.0, 1.0);

	if (uDebugLevel == 1)
		outFragColor = vec4(FrontPosition, 1.0);
	else
		outFragColor = ColorAccumulator;
}
