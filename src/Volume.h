#pragma once
#include <iostream>
#include <format>

#include "glm/glm.hpp"

#include "FrustumCulling.h"

struct Volume
{
	Volume() = default;
	virtual ~Volume() = default;

	virtual bool isOnFrustum(const FrustumCulling& pFrustumCulling, 
							 const glm::mat4& pModelMatrix) = 0;
};

class Sphere : public Volume
{
public:
	Sphere() = default;

	bool isOnFrustum(const FrustumCulling& pFrustumCulling,
					 const glm::mat4& pModelMatrix) override;
	
private:
	glm::vec3 mCenter{ 0.0f };
	float mRadius{ 0.0f };

};
