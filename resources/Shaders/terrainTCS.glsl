#version 430 core

layout (vertices = 4) out;

out vec3 WorldPos[];
out vec2 TextureCoord[];
out float Height[];

in vec2 texCoord[];
in vec3 worldPos[];
in float height[];

uniform mat4 uMVP;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	TextureCoord[gl_InvocationID] = texCoord[gl_InvocationID];
	WorldPos[gl_InvocationID] = worldPos[gl_InvocationID]; 
	Height[gl_InvocationID] = height[gl_InvocationID];

	if(gl_InvocationID == 0)
	{
		const int MIN_TESS_LEVEL = 4;
		const int MAX_TESS_LEVEL = 64;
		const float MIN_DISTANCE = 20.0f;
		const float MAX_DISTANCE = 800.0f;

		vec4 eyeSpace00 = uView * uModel * gl_in[0].gl_Position;
		vec4 eyeSpace01 = uView * uModel * gl_in[1].gl_Position;
		vec4 eyeSpace10 = uView * uModel * gl_in[2].gl_Position;
		vec4 eyeSpace11 = uView * uModel * gl_in[3].gl_Position;

		float distance00 = clamp((abs(eyeSpace00.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0f, 1.0f);
		float distance01 = clamp((abs(eyeSpace01.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0f, 1.0f);
		float distance10 = clamp((abs(eyeSpace10.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0f, 1.0f);
		float distance11 = clamp((abs(eyeSpace11.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0f, 1.0f);

		float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00));
		float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01));
		float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11));
		float tessLevel3 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10));

		gl_TessLevelOuter[0] = tessLevel0;
		gl_TessLevelOuter[1] = tessLevel1;
		gl_TessLevelOuter[2] = tessLevel2;
		gl_TessLevelOuter[3] = tessLevel3;

		gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
		gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
	}
}