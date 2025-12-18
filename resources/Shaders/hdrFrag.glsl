#version 430 core

out vec4 outFragColor;

in vec2 fragTexCoord;

uniform sampler2D uHDRBuffer;
uniform int uHDR;
uniform float uExposure;

void main()
{
	const float gamma = 2.2f;
	vec3 hdrColor = texture(uHDRBuffer, fragTexCoord).rgb;
	if (uHDR == 1)
	{
		// reinhard
		// vec3 result = hdrColor / (hdrColor + vec3(1.0f));
		vec3 result = vec3(1.0f) - exp(-hdrColor * uExposure);
		result = pow(result, vec3(1.0f / gamma));
		outFragColor = vec4(result, 1.0f);
	}
	else 
	{
		vec3 result = pow(hdrColor, vec3(1.0f / gamma));
		outFragColor = vec4(result, 1.0f);
	}
}