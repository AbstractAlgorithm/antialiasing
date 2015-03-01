#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

uniform vec2 resolution;

in vec2 uv[];
out vec2 _A;
out vec2 _B;
out vec2 _C;
out vec2 vUV;

void main()
{
	vec2 aspect = vec2(resolution.x/resolution.y,1.0);
	_A = gl_in[0].gl_Position.xy*aspect;
	_B = gl_in[1].gl_Position.xy*aspect;
	_C = gl_in[2].gl_Position.xy*aspect;

	vUV = uv[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	vUV = uv[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	vUV = uv[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	
	EndPrimitive();
}