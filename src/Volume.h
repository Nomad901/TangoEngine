#pragma once
#include <iostream>
#include <format>

#include "glm/glm.hpp"

#include "FrustumCulling.h"

struct Volume
{
	Volume() = default;
	virtual ~Volume() = default;

	virtual bool isOnFrustum(FrustumCulling& pFrustumCulling, 
							 const glm::mat4& pModelMatrix) = 0;
};

class Sphere : public Volume
{
public:
	Sphere(const glm::vec3& pCenter, float pRadius);

	bool isOnForwardPlane(const Plane& pPlane);
	bool isOnFrustum(FrustumCulling& pFrustumCulling,
					 const glm::mat4& pModelMatrix) override;
	
private:
	glm::vec3 mCenter{ 0.0f };
	float mRadius{ 0.0f };

};

class AABB : public Volume
{
public:
	AABB(const glm::vec3& pMax, const glm::vec3& pMin);
	AABB(const glm::vec3& pCenter, float pLX, float pLY, float pLZ);

	std::array<glm::vec3, 8> getVertices() const;

	bool isOnForwardPlane(const Plane& pPlane) const;
	bool isOnFrustum(FrustumCulling& pFrustumCulling,
					 const glm::mat4& pModelMatrix) override;

private:
	glm::vec3 mCenter{ 0.0f };
	glm::vec3 mExtents{ 0.0f };
};
