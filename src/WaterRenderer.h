#pragma once
#include <memory>
#include <vector>

#include "Water.h"
#include "WaterShader.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Camera.h"
#include "Transform.h"
#include "WaterFBO.h"
#include "GBuffer.h"

class WaterRenderer
{
public:
	WaterRenderer() = default;
	WaterRenderer(const std::filesystem::path& pVertPath, 
				  const std::filesystem::path& pFragPath,
				  const std::filesystem::path& pDuDvMap,
				  const std::filesystem::path& pNormalMap);
	
	void init(const std::filesystem::path& pVertPath,
			  const std::filesystem::path& pFragPath, 
			  const std::filesystem::path& pDuDvMap,
			  const std::filesystem::path& pNormalMap);
	
	void render(const std::vector<Water>& pWaterTiles,
				Camera& pCamera, const glm::mat4& pProjMat, 
				GBuffer& pGBuffer, DirectionalLight& pDirectionalLight,
				float pNearPlane, float pFarPlane,
				const glm::vec4& pWaterColor);

private:
	void bind(Camera& pCamera, const glm::mat4& pProjMat,
			  GBuffer& pGBuffer, DirectionalLight& pDirectionalLight,
			  float pNearPlane, float pFarPlane,
			  const glm::vec4& pWaterColor);
	void renderQuad();

	void setUpQuad();
	
private:
	uint32_t mVAO{}, mVBO{};
	const float WAVE_SPEED = 0.003f;
	float mMoveFactor{};

	Transform mQuadTransform;
	WaterShader mWaterShader;

	Texture2 mDuDvWaterTexture;
	Texture2 mNormalMap;
};