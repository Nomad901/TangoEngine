#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;
layout (location = 3) in ivec4 boneID;
layout (location = 4) in ivec2 boneID2;
layout (location = 5) in vec4 boneWeights;
layout (location = 6) in vec2 boneWeights2;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormals;
flat out vec4 fragBoneID;
flat out vec2 fragBoneID2;
out vec4 fragBoneWeights;
out vec2 fragBoneWeights2;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(pos, 1.0f);
	
	fragPos = pos;
	fragTexCoord = texCoord;
	fragNormals = normals;
	fragBoneID = vec4(boneID);
	fragBoneID2 = vec2(boneID2);
	fragBoneWeights = boneWeights;
	fragBoneWeights2 = boneWeights2;
}