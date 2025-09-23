#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
	vec3 normal;
} vs_in[];

const float magnitude = 0.4f;

uniform mat4 uProj;

void generateLine(int pIndex)
{
	gl_Position = uProj * gl_in[pIndex].gl_Position;
	EmitVertex();
	gl_Position = uProj * (gl_in[pIndex].gl_Position + vec4(vs_in[pIndex].normal, 0.0f) * magnitude);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	generateLine(0);
	generateLine(1);
	generateLine(2);
}