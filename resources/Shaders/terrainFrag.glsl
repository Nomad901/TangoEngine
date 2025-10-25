#version 430 core

out vec4 fragColor;

in vec4 Color;
in vec2 TexCoord;
in vec3 worldPos;
in vec3 Normals;
in float LightFactor;

//
// UNIFORMS TEXTURES
//
uniform sampler2D uTextureHeight0;
uniform sampler2D uTextureHeight1;
uniform sampler2D uTextureHeight2;
uniform sampler2D uTextureHeight3;

// 
// UNIFORMS HEIGHTS
//
uniform float uHeight0 = 20.0f;
uniform float uHeight1 = 50.0f;
uniform float uHeight2 = 80.0f;
uniform float uHeight3 = 130.0f;

// 
// UNIFORM CHECK-COLOR
//
uniform int isSingleTex;

vec4 calculateTexColor()
{
	vec4 texColor;
	float height = worldPos.y;

	if (height < uHeight0)
		texColor = texture(uTextureHeight0, TexCoord);
	else if (height < uHeight1)
	{
		vec4 color0  = texture(uTextureHeight0, TexCoord);
		vec4 color1  = texture(uTextureHeight1, TexCoord);
		float delta  = uHeight1 - uHeight0;
		float factor = (height - uHeight0) / delta;
		texColor = mix(color0, color1, factor);
	}
	else if (height < uHeight2)
	{
		vec4 color0  = texture(uTextureHeight1, TexCoord);
		vec4 color1  = texture(uTextureHeight2, TexCoord);
		float delta  = uHeight2 - uHeight1;
		float factor = (height - uHeight1) / delta;
		texColor = mix(color0, color1, factor);
	}
	else if (height < uHeight3)
	{
		vec4 color0  = texture(uTextureHeight2, TexCoord);
		vec4 color1  = texture(uTextureHeight3, TexCoord);
		float delta  = uHeight3 - uHeight2;
		float factor = (height - uHeight2) / delta;
		texColor = mix(color0, color1, factor);
	}
	else 
		texColor = texture(uTextureHeight3, TexCoord);
	
	return texColor;
}

void main()
{
	if(isSingleTex == 1)
	{
		vec3 result = vec3(Color) * vec3(LightFactor);
		fragColor = vec4(result.x, result.y, result.z, 0.9f);
	}
	else 
	{
		vec4 texture_ = calculateTexColor();
		vec3 result = vec3(texture_) * vec3(LightFactor);
		fragColor = vec4(result.x, result.y, result.z, 0.9f);
	}
}
