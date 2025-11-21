#pragma once
#include <memory>
#include <vector>

#include "Water.h"
#include "WaterShader.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Camera.h"
#include "Transform.h"

class WaterRenderer
{
public:
	WaterRenderer() = default;
	WaterRenderer(const std::filesystem::path& pVertPath, 
				  const std::filesystem::path& pFragPath);
	
	void init(const std::filesystem::path& pVertPath,
			  const std::filesystem::path& pFragPath);

	void render(const std::vector<Water> pWaterTiles,
				Camera& pCamera, const glm::mat4& pProjMat);

private:
	void bind(Camera& pCamera, const glm::mat4& pProjMat);
	void renderQuad();

	void setUpQuad();
	
private:
	uint32_t mVAO{}, mVBO{};
	
	Transform mQuadTransform;
	WaterShader mWaterShader;

};

