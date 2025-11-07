#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;
layout (location = 3) in ivec4 boneID;
layout (location = 4) in vec4 boneWeights;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormals;
out vec4 fragBoneID;
out vec4 fragBoneWeights;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(pos, 1.0f);
	
	fragPos = pos;
	fragTexCoord = texCoord;
	fragNormals = normals;
	fragBoneID = vec4(boneID);
	fragBoneWeights = boneWeights;
}