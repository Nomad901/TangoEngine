#version 430 core

out vec4 fragColor;
 
in float height;
in vec3 normals;
in vec3 worldPos;

uniform sampler2D uGrassTex;
uniform sampler2D uRockTex;

void main()
{
	float slope = 1.0f - normals.y;

	vec3 color;

	if(slope < 0.3f)
	{
		color = texture(uGrassTex, worldPos.xz * 0.1f).rgb;
	}
	else if(slope < 0.7f)
	{
		vec3 grass = texture(uGrassTex, worldPos.xz * 0.1f).rgb;
		vec3 rock  = texture(uRockTex, worldPos.xz * 0.2f).rgb;
		float blend = (slope - 0.3f) / 0.4f;
		color = mix(grass, rock, blend);
	}
	else
	{
		color = texture(uRockTex, worldPos.xz * 0.2f).rgb;
	}

	fragColor = vec4(color, 1.0f);
}