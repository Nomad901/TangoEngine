#include "Volume.h"

// SphereVolume
SphereVolume::SphereVolume(const glm::vec3& pCenter, float pRadius)
	: mCenter(pCenter), mRadius(pRadius)
{}

bool SphereVolume::isOnForwardPlane(const Plane& pPlane)
{
	return pPlane.getSignedDistanceToPlane(mCenter) > -mRadius;
}

bool SphereVolume::isOnFrustum(FrustumCulling& pFrustumCulling, const glm::mat4& pModelMatrix)
{
	const glm::vec3 globalScale = glm::vec3(glm::length(glm::vec3(pModelMatrix[0][0], pModelMatrix[0][1], pModelMatrix[0][2])),
											glm::length(glm::vec3(pModelMatrix[1][0], pModelMatrix[1][1], pModelMatrix[1][2])),
											glm::length(glm::vec3(pModelMatrix[2][0], pModelMatrix[2][1], pModelMatrix[2][2])));
	const glm::vec3 globalCenter{ pModelMatrix * glm::vec4(mCenter, 1.0f) };
	const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

	SphereVolume globalSphereVolume(globalCenter, mRadius * (maxScale * 0.5f));
	
	return (globalSphereVolume.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::LEFT_FACE))  &&
			globalSphereVolume.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::RIGHT_FACE)) &&
			globalSphereVolume.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::NEAR_FACE))  &&
			globalSphereVolume.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::FAR_FACE))   &&
			globalSphereVolume.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::TOP_FACE))   &&
			globalSphereVolume.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::BOTTOM_FACE)));
}

// AABB
AABB::AABB(const glm::vec3& pMax, const glm::vec3& pMin)
	: mCenter((pMax + pMin) * 0.5f), mExtents(pMax.x - mCenter.x, pMax.y - mCenter.y, pMax.z - mCenter.z)
{}

AABB::AABB(const glm::vec3& pCenter, float pLX, float pLY, float pLZ)
	: mCenter(pCenter), mExtents(pLX, pLY, pLZ)
{}

std::array<glm::vec3, 8> AABB::getVertices() const
{
	std::array<glm::vec3, 8> vertices;
	vertices[0] = glm::vec3(mCenter.x - mExtents.x, mCenter.y - mExtents.y, mCenter.z - mExtents.z);
	vertices[1] = glm::vec3(mCenter.x + mExtents.x, mCenter.y - mExtents.y, mCenter.z - mExtents.z);
	vertices[2] = glm::vec3(mCenter.x - mExtents.x, mCenter.y + mExtents.y, mCenter.z - mExtents.z);
	vertices[3] = glm::vec3(mCenter.x + mExtents.x, mCenter.y + mExtents.y, mCenter.z - mExtents.z);
	vertices[4] = glm::vec3(mCenter.x - mExtents.x, mCenter.y - mExtents.y, mCenter.z + mExtents.z);
	vertices[5] = glm::vec3(mCenter.x + mExtents.x, mCenter.y - mExtents.y, mCenter.z + mExtents.z);
	vertices[6] = glm::vec3(mCenter.x - mExtents.x, mCenter.y + mExtents.y, mCenter.z + mExtents.z);
	vertices[7] = glm::vec3(mCenter.x + mExtents.x, mCenter.y + mExtents.y, mCenter.z + mExtents.z);
	return vertices;
}

bool AABB::isOnForwardPlane(const Plane& pPlane) const 
{
	const float radius = mExtents.x * std::abs(pPlane.getNormal().x) +
						 mExtents.y * std::abs(pPlane.getNormal().y) +
						 mExtents.z * std::abs(pPlane.getNormal().z);
	return -radius <= pPlane.getSignedDistanceToPlane(mCenter);
}

bool AABB::isOnFrustum(FrustumCulling& pFrustumCulling, const glm::mat4& pModelMatrix)
{
	const glm::vec3 globalCenter = glm::vec3(pModelMatrix * glm::vec4(mCenter, 1.0f));
	
	const glm::vec3 rightVec   =  pModelMatrix[0] * mExtents.x;
	const glm::vec3 upVec	   =  pModelMatrix[1] * mExtents.y;
	const glm::vec3 forwardVec = -pModelMatrix[2] * mExtents.z;

	const float newLX = std::abs(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), rightVec)) +
						std::abs(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), upVec)) +
						std::abs(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), forwardVec));
	const float newLY = std::abs(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), rightVec)) +
						std::abs(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), upVec)) +
						std::abs(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), forwardVec));
	const float newLZ = std::abs(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), rightVec)) +
						std::abs(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), upVec)) +
						std::abs(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), forwardVec));

	const AABB globalAABB(globalCenter, newLX, newLY, newLZ);

	return globalAABB.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::RIGHT_FACE))  &&
		   globalAABB.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::LEFT_FACE))	 &&
		   globalAABB.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::TOP_FACE))	 &&
		   globalAABB.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::BOTTOM_FACE)) &&
		   globalAABB.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::NEAR_FACE))	 &&
		   globalAABB.isOnForwardPlane(pFrustumCulling.getPlane(PlaneType::FAR_FACE));
}