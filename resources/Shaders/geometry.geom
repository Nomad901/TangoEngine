#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

// in
in DATA
{
	vec3 normal;
	vec3 color;
	vec2 texCoord;
	mat4 proj;
} data_in[];
in vec3 fragPos[];

// out
out DATA 
{
	vec3 normal;
	vec3 color;
	vec2 texCoord;
	mat4 proj;
} data_out;	
out vec3 outFragPos;

const float magnitude = 0.4f;

void generateLine(int pIndex)
{
	gl_Position = data_in[pIndex].proj * gl_in[pIndex].gl_Position;
	data_out.normal = data_in[0].normal;
	data_out.color = data_in[0].color;
	data_out.texCoord = data_in[0].texCoord;
	data_out.proj = data_in[0].proj;
	outFragPos = fragPos[0];
	EmitVertex();
	gl_Position = data_in[pIndex].proj * (gl_in[pIndex].gl_Position + 
										  vec4(data_in[pIndex].normal, 0.0f) * magnitude);
	data_out.normal = data_in[0].normal;
	data_out.color = data_in[0].color;
	data_out.texCoord = data_in[0].texCoord;
	data_out.proj = data_in[0].proj;
	outFragPos = fragPos[0];
	EmitVertex();
	EndPrimitive();
}

void main()
{
	generateLine(0);
	generateLine(1);
	generateLine(2);
}