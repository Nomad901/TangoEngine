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
const int MAX_NUMBER_OF_BONES = 100;
uniform mat4 uBones[MAX_NUMBER_OF_BONES];

void main()
{
	mat4 boneTransform =  uBones[boneID[0]] * boneWeights[0];
		 boneTransform += uBones[boneID[1]] * boneWeights[1];
		 boneTransform += uBones[boneID[2]] * boneWeights[2];
		 boneTransform += uBones[boneID[3]] * boneWeights[3];
		 boneTransform += uBones[boneID2[0]] * boneWeights2[0];
		 boneTransform += uBones[boneID2[1]] * boneWeights2[1];

	vec4 posWithBone = boneTransform * vec4(pos, 1.0f);
	gl_Position = uMVP * posWithBone;
	
	fragPos = pos;
	fragTexCoord = texCoord;
	fragNormals = normals;
	fragBoneID = vec4(boneID);
	fragBoneID2 = vec2(boneID2);
	fragBoneWeights = boneWeights;
	fragBoneWeights2 = boneWeights2;
}