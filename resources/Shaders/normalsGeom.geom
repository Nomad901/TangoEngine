#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices = 8) out;

in VS_OUT
{
	vec3 normal;
} vs_in[];

const float magnitude = 1.0f;

uniform mat4 uProj;

void generateLine(int pIndex)
{
	gl_Position = uProj * gl_in[pIndex].gl_Position;
	EmitVertex();
	gl_Position = uProj * (gl_in[pIndex].gl_Position + vec4(vs_in[pIndex].normal, 0.0f) * magnitude);
	EmitVertex();
	EndPrimitive();
}

void generateFaceNormals()
{
	vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0f;

	vec3 a = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
	vec3 faceNormal = normalize(cross(a, b));

	gl_Position = uProj * center;
	EmitVertex();
	gl_Position = uProj * (center + vec4(faceNormal, 0.0f) * magnitude);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	generateFaceNormals();
}