#version 430 core

out vec4 finalFragColor;

in vec2 fragTexCoord;

uniform sampler2D gPos;
uniform sampler2D gNormals;
uniform sampler2D gSpec;

struct Light
{
	vec3 position;
	vec3 color;
	
	float linear;
	float quadratic;
	float radius;
};

uniform Light uLight;
uniform vec3 uViewPos;

void main()
{
	vec3 fragPos = texture(gPos, fragTexCoord).rgb;
	vec3 normal = texture(gNormals, fragTexCoord).rgb;
	vec3 Diffuse = texture(gSpec, fragTexCoord).rgb;
	float Specular = texture(gSpec, fragTexCoord).a;

	vec3 lighting = Diffuse * 0.1f;
	vec3 viewDir = normalize(uViewPos - fragPos);

	float distance = length(uLight.position - fragPos);
	if(distance < uLight.radius)
	{
		vec3 lightDir = normalize(uLight.position - fragPos);
		vec3 diffuse = max(dot(normal, lightDir), 0.0f) * Diffuse * uLight.color;

		vec3 halfWayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfWayDir), 0.0f), 16.0f);
		vec3 specular = uLight.color * spec * Specular;

		float attenuation = 1.0f / (1.0f + uLight.linear * distance + uLight.quadratic * distance * distance);
		diffuse *= attenuation;
		specular *= attenuation;
		lighting += diffuse + specular;
	}
	finalFragColor = vec4(lighting, 1.0f);
}