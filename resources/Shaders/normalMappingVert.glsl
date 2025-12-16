#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;
out vec3 lightPos;
out vec3 viewPos;

uniform mat4 uModel;
uniform mat4 uViewMat;
uniform mat4 uProj;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main() 
{
	gl_Position = uProj * uViewMat * uModel * vec4(pos, 1.0f);
	fragPos = vec3(uModel * vec4(pos, 1.0f));
	fragTexCoord = texCoord;
	
	mat3 normalMatrix = transpose(inverse(mat3(uModel)));
	vec3 T = normalize(normalMatrix * tangent);
	vec3 N = normalize(normalMatrix * normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);
	
	mat3 TBN = transpose(mat3(T, B, N));
	tangentLightPos = TBN * uLightPos;
	tangentViewPos  = TBN * uViewPos;
	tangentFragPos  = TBN * fragPos;

	lightPos = uLightPos;
	viewPos = uViewPos;

}	