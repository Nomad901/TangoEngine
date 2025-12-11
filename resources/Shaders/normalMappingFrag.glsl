#version 430 core

out vec4 fragColor;

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;
in vec3 lightPos;
in vec3 viewPos;

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;

void main() 
{
	vec3 normal = texture(uNormalMap, fragTexCoord).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	
	vec3 color = texture(uDiffuseMap, fragTexCoord).rgb;
	vec3 ambient = 0.1f * color;
	
	vec3 lightDir = normalize(fragPos - lightPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;

	vec3 viewDir = normalize(fragPos - viewPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);

	vec3 specular = vec3(0.2f) * spec;
	fragColor = vec4(ambient + diffuse + specular, 1.0f);
}