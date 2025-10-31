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

const int MAX_LIGHTS = 32;
uniform Light lights[MAX_LIGHTS];
uniform int uNumLights;
uniform vec3 uViewPos;

void main()
{
    vec3 fragPos = texture(gPos, fragTexCoord).rgb;
    vec3 normal = texture(gNormals, fragTexCoord).rgb;
    vec3 albedo = texture(gSpec, fragTexCoord).rgb;  
    float specular = texture(gSpec, fragTexCoord).a;

    vec3 lighting = albedo * 0.1f;  
    vec3 viewDir = normalize(uViewPos - fragPos);

    for (int i = 0; i < uNumLights; i++)
    {
        float distance = length(lights[i].position - fragPos);
        if (distance < lights[i].radius)
        {
            vec3 lightDir = normalize(lights[i].position - fragPos);
            
            float diff = max(dot(normal, lightDir), 0.0f);
            vec3 diffuse = diff * albedo * lights[i].color;
            
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0f), 16.0f);
            vec3 specularColor = lights[i].color * spec * specular;

            float attenuation = 1.0f / (1.0f + lights[i].linear * distance + lights[i].quadratic * distance * distance);
            
            lighting += (diffuse + specularColor) * attenuation;
        }
    }

    //lighting = lighting / (lighting + vec3(1.0f));
    //lighting = pow(lighting, vec3(1.0f/2.2f)); 

    finalFragColor = vec4(lighting, 1.0f);
}