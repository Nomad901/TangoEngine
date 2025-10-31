#version 430 core

out vec4 fragColor;

//
// base light class in order to implement "parent-child" relation;
//
struct baseLight
{
	vec3 mColor;

	float mAmbientIntensity;
	float mDiffuseIntensity;
};

//
// directional light;
//
struct directionalLight
{
	baseLight mBaseLight;
	vec3 mDirection;
};

//
// attenuation;
//
struct attenuation
{
	float mConstant;
	float mLinear;
	float mExp;
};

//
// point light;
//
struct pointLight
{
	baseLight mBaseLight;
	vec3 mPos;
	attenuation mAttenuation;
};

//
// textures;
//
uniform sampler2D uPositionMap;
uniform sampler2D uColorMap;
uniform sampler2D uNormalMap;

//
// lights;
//
const int numberOfLights = 32; 
uniform directionalLight uDirectionalLight[numberOfLights];
uniform pointLight uPointLight[numberOfLights];
uniform int uNumberLightsToProcess; 
// todo: spotlight;

//
// light parameters; 
//
uniform float uMatSpecularIntensity;
uniform float uSpecularPower;
uniform int uLightType;

//
// other parameters;
// 
uniform vec3 uViewWorldPos;
uniform vec2 uScreenSize;

vec4 calcInternalLight(baseLight pBaseLight,
					   vec3 pLightDirection,
					   vec3 pWorldPos,
					   vec3 pNormal)
{
	vec4 ambientColor  = vec4(pBaseLight.mColor * pBaseLight.mAmbientIntensity, 1.0f);
	float diffuseFactor = dot(pNormal, -pLightDirection);

	vec4 diffuseColor  = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	if(diffuseFactor > 0.0f)
	{
		diffuseColor = vec4(pBaseLight.mColor * pBaseLight.mDiffuseIntensity * diffuseFactor, 1.0f);

		vec3 vertexToEye = normalize(uViewWorldPos - pWorldPos);
		vec3 lightReflect = normalize(reflect(pLightDirection, pNormal));
		float specularFactor = dot(vertexToEye, lightReflect);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, uSpecularPower);
			specularColor  = vec4(pBaseLight.mColor * uMatSpecularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 calcDirectionalLight(vec3 pWorldPos, vec3 pNormal, int pIndex)
{
	return calcInternalLight(uDirectionalLight[pIndex].mBaseLight,
							 uDirectionalLight[pIndex].mDirection,
							 pWorldPos, pNormal);
}

vec4 calcPointLight(vec3 pWorldPos, vec3 pNormal, int pIndex)
{
	vec3 lightDirection = pWorldPos - uPointLight[pIndex].mPos;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec4 color = calcInternalLight(uPointLight[pIndex].mBaseLight, lightDirection, pWorldPos, pNormal);
	
	float attenuationFactor = uPointLight[pIndex].mAttenuation.mConstant + 
							  uPointLight[pIndex].mAttenuation.mLinear * distance +
							  uPointLight[pIndex].mAttenuation.mExp * distance * distance;

	attenuationFactor = max(1.0f, attenuationFactor);

	return color / attenuationFactor;
}

// todo: calculate point light;

vec2 calcTexCoord()
{
	return gl_FragCoord.xy / uScreenSize;
}

void main()
{
	vec2 texCoord = calcTexCoord();
	vec3 worldPos = texture(uPositionMap, texCoord).xyz;
	vec3 color	  = texture(uColorMap, texCoord).xyz;
	vec3 normal   = texture(uNormalMap, texCoord).xyz;
	normal = normalize(normal);

	fragColor = vec4(color, 1.0f) * calcPointLight(worldPos, normal, 0);
}