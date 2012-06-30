varying vec3 vColor;
varying vec4 vPosition;

uniform sampler2D uBackPosition;
uniform sampler3D uVolumeData;

uniform mat4 uModelMatrix;

uniform vec3 uCameraPosition;

uniform float uAlphaIntensity;

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

vec3 penter;
vec3 pexit;

bool rayAABBIntersect(vec3 start, vec3 dir, vec3 Min, vec3 Max)
{
    enter = 0.0f;
    Exit = 1.0f;

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

void main()
{
	vec2 texc = ((vPosition.xy / vPosition.w) + 1.0) / 2.0;

	vec3 BackPosition = vColor;

	vec3 FrontPosition;

	vec4 CameraPosition = inverse(uModelMatrix) * vec4(uCameraPosition, 1.0);


	int Debug = 0;

	// Calculate surface point
	if (CameraPosition.x >= -0.5 && 
		CameraPosition.y >= -0.5 && 
		CameraPosition.z >= -0.5 && 
		CameraPosition.x <=  0.5 && 
		CameraPosition.y <=  0.5 && 
		CameraPosition.z <=  0.5)
	{
		FrontPosition = CameraPosition;
		if (Debug)
		{
			gl_FragColor = vec4(1, 0, 0, 1);
			return;
		}
	}
	else if (rayAABBIntersect(CameraPosition, (BackPosition - vec3(0.5)) - CameraPosition, vec3(-0.5), vec3(0.5)))
	{
		FrontPosition = penter;
		if (Debug)
		{
			gl_FragColor = vec4(0, 1, 0, 1);
			return;
		}
	}
	else
	{
		FrontPosition = CameraPosition;
		if (Debug)
		{
			gl_FragColor = vec4(0, 0, 1, 1);
			return;
		}
	}

	FrontPosition += vec3(0.5);

	float stepsize = 1.0 / 50.0;

	vec3 start = FrontPosition;
	vec3 dir = BackPosition - FrontPosition;

	float len = length(dir.xyz); // the length from front to back is calculated and used to terminate the ray
	vec3 norm_dir = normalize(dir);
	float delta = stepsize;
	vec3 delta_dir = norm_dir * delta;
	float delta_dir_len = length(delta_dir);
	vec3 vec = start;
	vec4 col_acc = vec4(0,0,0,0);
	float alpha_acc = 0;
	float length_acc = 0;

	for(int i = 0; i < 1000; i ++)
	{
		//vec4 color_sample = vec4(vec, 0.5);
		vec4 color_sample = texture3D(uVolumeData, vec);
		float alpha_sample = color_sample.a * stepsize * uAlphaIntensity;
		col_acc   += (1.0 - alpha_acc) * color_sample * alpha_sample * 3;
		//col_acc   += color_sample;
		alpha_acc += alpha_sample;
		vec += delta_dir;
		length_acc += delta_dir_len;
		if (length_acc >= len || alpha_acc > 1.0)
			break; // terminate if opacity > 1 or the ray is outside the volume
	}

    gl_FragColor = col_acc;
}
