#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;
layout (location = 3) in ivec4 boneID;
//layout (location = 4) in ivec2 boneID2;
layout (location = 4) in vec4 boneWeights;
//layout (location = 6) in vec2 boneWeights2;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormals;
flat out vec4 fragBoneID;
//flat out vec2 fragBoneID2;
out vec4 fragBoneWeights;
//out vec2 fragBoneWeights2;

uniform mat4 uMVP;
const int MAX_NUMBER_OF_BONES = 100;
uniform mat4 uBones[MAX_NUMBER_OF_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0);
    
    totalPosition += uBones[boneID[0]] * vec4(pos, 1.0) * boneWeights[0];
    totalPosition += uBones[boneID[1]] * vec4(pos, 1.0) * boneWeights[1];
    totalPosition += uBones[boneID[2]] * vec4(pos, 1.0) * boneWeights[2];
    totalPosition += uBones[boneID[3]] * vec4(pos, 1.0) * boneWeights[3];

    if (totalPosition == vec4(0.0)) 
        totalPosition = vec4(pos, 1.0);

    //if (boneWeights2[0] > 0.0)
    //    totalPosition += uBones[boneID2[0]] * vec4(pos, 1.0) * boneWeights2[0];
    //if (boneWeights2[1] > 0.0)
    //    totalPosition += uBones[boneID2[1]] * vec4(pos, 1.0) * boneWeights2[1];
    
    gl_Position = uMVP * totalPosition;
    //gl_Position = uMVP * vec4(pos, 1.0f);
    
    fragPos = pos;
    fragTexCoord = texCoord;
    fragNormals = fragNormals = mat3(transpose(inverse(uBones[boneID[0]]))) * normals * boneWeights[0] +
                                mat3(transpose(inverse(uBones[boneID[1]]))) * normals * boneWeights[1] +
                                mat3(transpose(inverse(uBones[boneID[2]]))) * normals * boneWeights[2] +
                                mat3(transpose(inverse(uBones[boneID[3]]))) * normals * boneWeights[3];
    fragBoneID = boneID;
    //fragBoneID2 = boneID2;
    fragBoneWeights = boneWeights;
    //fragBoneWeights2 = boneWeights2;
}
