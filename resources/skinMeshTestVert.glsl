#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;
layout (location = 3) in ivec4 boneID;
layout (location = 4) in ivec4 boneID2;
layout (location = 5) in vec4 boneWeights;
layout (location = 6) in vec4 boneWeights2;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormals;

uniform mat4 uMVP;
const int MAX_NUMBER_OF_BONES = 100;
uniform mat4 uBones[MAX_NUMBER_OF_BONES];

void main()
{
    mat4 boneTransform = mat4(0.0f);
    float totalWeight = 0.0f;

    for(int i = 0; i < 4; ++i)
    {
        if(boneID[i] >= 0 && boneWeights[i] > 0.0f)
        {
            boneTransform += uBones[boneID[i]] * boneWeights[i];
            totalWeight += boneWeights[i];
        }
    }
    for(int i = 0; i < 4; ++i)
    {
        if(boneID2[i] >= 0 && boneWeights2[i] > 0.0f)
        {
            boneTransform += uBones[boneID2[i]] * boneWeights2[i];
            totalWeight += boneWeights2[i];
        }
    }
    
    if(totalWeight == 0.0f)
        boneTransform = mat4(1.0f);

    vec4 totalPosition = boneTransform * vec4(pos, 1.0f);
    vec3 transformedNormal = mat3(boneTransform) * normals;

    gl_Position = uMVP * totalPosition;
    
    fragPos = pos;
    fragTexCoord = texCoord;
    fragNormals = transformedNormal;
}