#pragma once
#include <array>

#include "glm/glm.hpp"

#include "Utils.h"
#include "Camera.h"

enum class PlaneType
{
	TOP_FACE = 0,
	BOTTOM_FACE = 1,
	LEFT_FACE = 2,
	RIGHT_FACE = 3,
	FAR_FACE = 4,
	NEAR_FACE = 5
};

class Plane
{
public:
	Plane() = default;
	Plane(const glm::vec3& pPoint, const glm::vec3& pNormal);
	
	float getSignedDistanceToPlane(const glm::vec3& pPoint) const;

	const glm::vec3& getNormal() const noexcept;
	float getDistance() const noexcept;

private:
	glm::vec3 mNormal{ 0.0f, 1.0f, 0.0f };
	float mDistance{ 0.0f };
};

class FrustumCulling
{
public:
	FrustumCulling();
	FrustumCulling(Camera* pCamera, float pAspect, float pFovY,
				   float pZNear, float pZFar);

	void initFrustumFromCamera(Camera* pCamera, float pAspect, float pFovY,
													  float pZNear, float pZFar);
	Plane& getPlane(PlaneType pPlaneType) noexcept;

private:
	std::array<Plane, 6> mPlanes;

};

