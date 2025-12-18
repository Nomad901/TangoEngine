#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texCoord;
layout (location = 4) in float lightFactor;

out vec3 fragPos;
out vec3 fragNormals;
out vec2 fragTexCoord;

uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uViewMat;

uniform int uInverseNormals;

void main()
{
	gl_Position = uProj * uViewMat * uModel * vec4(pos, 1.0f);
	fragPos = vec3(uModel * vec4(pos, 1.0f));
	fragTexCoord = texCoord;

	vec3 normal = uInverseNormals == 1? -normals : normals;

	mat3 normalMatrix = transpose(inverse(mat3(uModel)));
	fragNormals = normalize(normalMatrix * normal);
}
