#pragma once
#include <array>

#include "glm/glm.hpp"

#include "Utils.h"
#include "Camera.h"
#include "Transform.h"

class Plane
{
public:
	Plane() = default;
	Plane(const glm::vec3& pPoint, const glm::vec3& pNormal);
	
	float getSignedDistanceToPlane(const glm::vec3& pPoint) const;

	const glm::vec4& getClipPlaneVec() const noexcept;
	void setClipPlaneVec(const glm::vec4& pPlaneVec4);

	const glm::vec3& getNormal() const noexcept;
	float getDistance() const noexcept;

private:
	glm::vec3 mNormal{ 0.0f, 1.0f, 0.0f };
	glm::vec4 mCLipPlaneVec{ 0.0f };
	float mDistance{ 0.0f };
};


class FrustumCulling
{
public:
	enum class PlaneType
	{
		TOP_FACE = 0,
		BOTTOM_FACE = 1,
		LEFT_FACE = 2,
		RIGHT_FACE = 3,
		FAR_FACE = 4,
		NEAR_FACE = 5,
		NUM_PLANES = 6
	};
public:
	FrustumCulling() = default;
	FrustumCulling(const glm::mat4& pViewProjMat);

	void update(const glm::mat4& pViewProjMat);

	bool isPointInsideViewFrustum(const glm::vec3& pPoint) const;
	bool isAABBInsideViewFrustum(const glm::vec3& pMinBounds, 
								 const glm::vec3& pMaxBounds) const;

	Plane& getPlane(PlaneType pPlaneType) noexcept;

private:
	size_t getIndex(PlaneType pPlaneType);

private:
	std::array<Plane, static_cast<size_t>(PlaneType::NUM_PLANES)> mPlanes;
};

//class FrustumCulling
//{
//public:
//	FrustumCulling();
//	FrustumCulling(Camera* pCamera, float pAspect, float pFovY,
//				   float pZNear, float pZFar);
//
//	void initFrustumFromCamera(Camera* pCamera, float pAspect, float pFovY,
//													  float pZNear, float pZFar);
//	Plane& getPlane(PlaneType pPlaneType) noexcept;
//
//private:
//	std::array<Plane, 6> mPlanes;
//
//};

