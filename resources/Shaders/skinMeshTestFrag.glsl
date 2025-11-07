#version 430 core

out vec4 fragColor;

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormals;
flat in vec4 fragBoneID;
in vec4 fragBoneWeights;

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

struct baseLight
{
	vec3 mColor;
	float mAmbientIntensity;
	float mDiffuseIntensity;
}; 

struct attenuation
{
	float mConstant;
	float mLinear;
	float mExp;
};

struct directionalLight
{
	baseLight mBaseLight;
	vec3 mDirection;
};

struct pointLight
{
	baseLight mBaseLight;
	attenuation mAttenuation;
	vec3 mPos;
};

struct spotLight
{
	pointLight mPointLight;
	vec3 mDirection;
	float mCutOff;
}

struct material 
{
	vec3 mAmbientColor;
	vec3 mDiffuseColor;
	vec3 mSpecularColor;
};

uniform directionalLight uDirectionalLight;
uniform pointLight uPointLight[MAX_POINT_LIGHTS];
uniform spotLight uSpotLight[MAX_SPOT_LIGHTS];
uniform int uNimberPointLights;
uniform int uNumberSpotLights;

uniform Material uMaterial;

uniform sampler2D uSampler;
uniform sampler2D uSamplerSpecularComponent;

uniform vec3 uCameraPos;

uniform int uDisplayBoneIndex;
uniform int uNumberOfBones;

vec4 calcLightInternal(baseLight pBaseLight, vec3 pLightDirection, vec3 pNormal)
{
	vec4 ambientColor = vec4(pBaseLight.mColor, 1.0f) * 
						pBaseLight.mAmbientIntensity * 
						vec4(uMaterial.mAmbientColor, 1.0f);

	float diffuseFactor = dot(pNormal, -pLightDirection);
	
	vec4 diffuseColor  = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	if (diffuseFactor > 0.0f)
	{
		diffuseColor = vec4(pBaseLight.color, 1.0f) * 
					   pBaseLight.mDiffuseIntensity *
					   vec4(uMaterial.mDiffuseColor, 1.0f) * 
					   diffuseFactor;

		vec3 pixelToCamera = normalize(uCameraPos - fragPos);
		vec3 lightReflect  = normalize(reflect(pLightDirection, pNormal));
		float specularFactor = dot(pixelToCamera, lightReflect);
		if (specularColor > 0.0f)
		{
			float specularExponent = texture(uSamplerSpecularComponent, fragTexCoord).r * 255.0f;
			specularFactor = pow(specularColor, specularExponent);
			specularColor = vec4(pBaseLight.color, 1.0f) *
							pBaseLight.mDiffuseIntensity * 
							vec4(uMaterial.mSpecularColor, 1.0f) * 
							specularFactor;
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 calcDirectionalLight(vec3 pNormal)
{
	return calcLightInternal(uDirectionalLight.mBaseLight, uDirectionalLight.mDirection, pNormal);
}

vec4 calcPointLight(pointLight pPointLight, vec3 pNormal)
{
	vec3 lightDirection = fragPos - pPointLight.mPos;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec4 color = calcLightInternal(pPointLight.mBaseLight, lightDirection, pNormal);
	float tmpAttenuation = pPointLight.mAttenuation.mConstant +
						   pPointLight.mAttenuation.mLinear * distance + 
						   pPointLight.mAttenuation.mExp * distance * distance;
	return color / tmpAttenuation;
}

vec4 calcSpotLight(spotLight pSpotLight, vec3 pNormal)
{
	vec3 lightToPixel = normalize(fragPos - pSpotLight.mPointLight.mPos);
	float spotFactor = dot(lightToPixel, pSpotLight.mDirection);

	if(spotFactor > pSpotLight.mCutOff)
	{
		vec4 color = calcPointLight(pSpotLight.mPointLight, pNormal);
		float spotLightIntensity = (1.0f - (1.0f - spotFactor)/(1.0f - pSpotLight.mCutOff));
		return color * spotLightIntensity * 1.5f;
	}
	else 
	{
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

void main()
{
	vec3 normal = normalize(fragNormals);
	vec4 totalLight = calcDirectionalLight(normal);

	for (int i = 0; i < uNimberPointLights; ++i)
	{
		totalLight += calcPointLight(uPointLight[i], normal);
	}
	for (int i = 0; i < uNumberSpotLights; ++i)
	{
		totalLight += calcSpotLight(uSpotLight[i], normal);
	}

	bool found = false;

	for(int i = 0; i < uNumberOfBones; ++i)
	{
		if(fragBoneID[i] >= uDisplayBoneIndex)
		{
			if(fragBoneWeights[i] >= 0.7f)
				fragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f) * fragBoneWeights[i];
			else if(fragBoneWeights[i] >= 0.4f && fragBoneWeights[i] <= 0.6f)
				fragColor = vec4(0.0f, 1.0f, 0.0f, 0.0f) * fragBoneWeights[i];
			else if(fragBoneWeights[i] >= 0.1f)
				fragColor = vec4(1.0f, 1.0f, 0.0f, 0.0f) * fragBoneWeights[i];
			
			found = true;
			break;
		}
	}

	if(!found)
	{
		fragColor = texture(uSampler, fragTexCoord) * totalLight * vec4(0.0001f) + vec4(0.0f, 0.0f, 1.0f, 0.0f);
	}
}