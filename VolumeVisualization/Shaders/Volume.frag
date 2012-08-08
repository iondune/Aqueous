varying vec3 vColor;
varying vec4 vPosition;

uniform sampler2D uBackPosition;
uniform sampler3D uVolumeData;

uniform float uAlphaIntensity;

void main()
{
	vec2 texc = ((vPosition.xy / vPosition.w) + 1.0) / 2.0;

	vec3 BackPosition = texture2D(uBackPosition, texc).xyz;
	vec3 FrontPosition = vColor;

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

	//normalize(col_acc);
    //gl_FragColor = vec4(texture3D(uVolumeData, vec3(0.5, 0.5, 0.5)).rgb, 1.0);
    //gl_FragColor = vec4(col_acc.rgb, 1.0);
    gl_FragColor = col_acc;//vec4(col_acc.rg, 1.0, 0.5);
    //gl_FragColor = vec4(BackPosition, 0.75);
    //gl_FragColor = vec4(norm_dir * 0.5 + 0.5, 0.75);
}
