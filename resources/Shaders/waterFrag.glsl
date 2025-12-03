#version 430 core

out vec4 fragColor;

in vec2 fragTexCoord;

in mat4 fragProjection;
in mat4 fragView;

uniform sampler2D uFinalImage;	   // DIFFUSE_TEXTURE  (GBuffer)
uniform sampler2D uPosition;	   // POSITION_TEXTURE (GBuffer)
uniform sampler2D uWaterNormal;	   // NORMAL_TEXTURE   (GBuffer)
uniform sampler2D uExtraComponent; // EXTRA_COMPONENT_TEXTURE (WaterGBuffer)
uniform sampler2D uColorBuffer;	   // COLOR_BUFFER_TEXTURE (WaterGBuffer)

uniform mat4 uInvView;
uniform mat4 uInvProjection;

const float rayStep = 0.1f;
const float minRayStep = 0.1f;
const float searchDist = 5.0f;
const float reflectionSpecularFallofExponent = 3.0f;
const int maxStep = 30;
const int numBinarySearchSteps = 5;

const vec3 scale = vec3(0.8f);
const float k = 19.19f;

vec4 rayCast(in vec3 pDir, inout vec3 pHitCoord, out float pDepth);
vec3 binarySearch(inout vec3 pDir, inout vec3 pHitCoord, inout float pDepth);
vec3 fresnelSchlick(in float pCosTheta, in vec3 pF0);
vec3 hash(vec3 pVector);

void main()
{
	vec2 metallicEmissive = texture(uExtraComponent, fragTexCoord).rg;
	float metallic = metallicEmissive.x;

	vec3 waterNormal = vec3(texture(uWaterNormal, fragTexCoord).rgb * 2.0f - 1.0f);
	vec3 viewNormal = normalize((fragView * vec4(waterNormal, 0.0f)).xyz);
	vec3 viewPos = textureLod(uPosition, fragTexCoord, 2).xyz; // should use third mip map level;
	vec3 albedo = texture(uFinalImage, fragTexCoord).rgb; // for fresnel;
	float spec = texture(uColorBuffer, fragTexCoord).w;

	vec3 F0 = vec3(0.03f);
	F0 = mix(F0, albedo, spec);

	vec3 fresnel = fresnelSchlick(max(dot(normalize(viewNormal), normalize(viewPos)), 0.0f), F0);
		
	vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));
	vec3 hitPos = viewPos;
	float depth;

	vec3 worldPos = vec3(vec4(viewPos, 1.0f) * uInvView);
	vec3 jitt = mix(vec3(0.0f), hash(worldPos), spec);

	vec4 coords = rayCast(jitt + reflected * max(minRayStep, -viewPos.z), hitPos, depth);

	// for erasing artficats on the edges of the screen;
	vec2 dCoords = smoothstep(0.2f, 0.6f, abs(vec2(0.5f) - coords.xy)); 
	float screenEdgeFactor= clamp(1.0f - (dCoords.x + dCoords.y), 0.0f, 1.0f);
	float multiplier = pow(metallic, reflectionSpecularFallofExponent) * screenEdgeFactor * (-reflected.z);
	
	vec3 SSR = texture(uFinalImage, coords.xy).rgb * clamp(multiplier, 0.0f, 0.9f) * fresnel;

	fragColor = vec4(SSR, 1.0f);
}

vec4 rayCast(in vec3 pDir, inout vec3 pHitCoord, out float pDepth)
{
	pDir *= rayStep;

	float depth = 0.0f;
	int steps = 0;
	vec4 projectedCoord = vec4(0.0f);

	for (int i = 0; i < maxStep; ++i)
	{
		pHitCoord += pDir;

		projectedCoord = fragProjection * vec4(pHitCoord, 1.0f);
		projectedCoord.xy /= projectedCoord.w;
		projectedCoord.xy = projectedCoord.xy * 0.5f + 0.5f;

		depth = textureLod(uPosition, projectedCoord.xy, 2).z;

		if (depth > 1000.0f) // maybe an object is in front of screen
			continue;

		pDepth = (pHitCoord - depth).z;

		if ((pDir.z - pDepth) < 1.2)
		{
			if (pDepth <= 0.0f)
			{
				vec4 result;
				vec3 binarySearchResult = binarySearch(pDir, pHitCoord, pDepth);
				result = vec4(binarySearchResult, 1.0f);
			}
			steps++;
		}
	}

	return vec4(projectedCoord.xy, depth, 1.0f);
}


vec3 binarySearch(inout vec3 pDir, inout vec3 pHitCoord, inout float pDepth)
{
	float depth = 0.0f;
	vec4 projectedCoord = vec4(0.0f);

	for (int i = 0; i < numBinarySearchSteps; ++i)
	{
		projectedCoord = fragProjection * vec4(pHitCoord, 1.0f);
		projectedCoord.xy /= projectedCoord.w;
		projectedCoord.xy = projectedCoord.xy * 0.5f + 0.5f;
		
		depth = textureLod(uPosition, projectedCoord.xy, 2).z;

		pDepth = (pHitCoord - depth).z;

		pDir += 0.5f;
		if(pDepth > 0.0f)
			pHitCoord += pDir;
		else 
			pHitCoord -= pDir;
	}

	projectedCoord = fragProjection * vec4(pHitCoord, 1.0f);
	projectedCoord.xy /= projectedCoord.w;
	projectedCoord.xy = projectedCoord.xy * 0.5f + 0.5f;
	
	return vec3(projectedCoord.xy, depth);
}

vec3 fresnelSchlick(in float pCosTheta, in vec3 pF0)
{
	return pF0 + (1.0f - pF0) * pow(1.0f - pCosTheta, 5.0f);
}

vec3 hash(vec3 pVector)
{
	pVector = fract(pVector * scale);
	pVector += dot(pVector, pVector.xyz + k);
	return fract((pVector.xxy + pVector.yxx) * pVector.zyx);
}