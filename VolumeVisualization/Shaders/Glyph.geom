
#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

uniform mat4 View;
uniform mat4 Projection;

in vec3 geColor[];

out vec3 fColor;


void MakePoint(vec3 CameraRight, vec3 CameraUp, vec2 Offset)
{
	const vec2 BillboardSize = vec2(0.02);

	vec3 Vertex = gl_in[0].gl_Position.xyz
		+ CameraRight * Offset.x * BillboardSize.x * 0.5
		+ CameraUp * Offset.y * BillboardSize.y * 0.5;
	gl_Position = Projection * View * vec4(Vertex, 1.0);
	EmitVertex();
}

void main()
{
	vec3 CameraRight = vec3(View[0][0], View[1][0], View[2][0]);
	vec3 CameraUp = vec3(View[0][1], View[1][1], View[2][1]);

	fColor = geColor[0];

	MakePoint(CameraRight, CameraUp, vec2(-1,  1));
	MakePoint(CameraRight, CameraUp, vec2( 1,  1));
	MakePoint(CameraRight, CameraUp, vec2( 1, -1));
	MakePoint(CameraRight, CameraUp, vec2(-1, -1));
	MakePoint(CameraRight, CameraUp, vec2(-1,  1));

	EndPrimitive();
}
