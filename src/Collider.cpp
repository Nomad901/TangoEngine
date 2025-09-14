#include "Collider.h"

bool Collider::areCollided(const Mesh& pMesh1, const Mesh& pMesh2)
{
	glm::vec3 minPoint1 = getMinPoint(pMesh1.getPos(), pMesh1.getSize());
	glm::vec3 maxPoint1 = getMaxPoint(pMesh1.getPos(), pMesh1.getSize());
	glm::vec3 minPoint2 = getMinPoint(pMesh2.getPos(), pMesh2.getSize());
	glm::vec3 maxPoint2 = getMaxPoint(pMesh2.getPos(), pMesh2.getSize());
	
	bool collideX = minPoint1.x <= maxPoint2.x && maxPoint1.x >= minPoint2.x;
	bool collideY = minPoint1.y <= maxPoint2.y && maxPoint1.y >= minPoint2.y;
	bool collideZ = minPoint1.z <= maxPoint2.z && maxPoint1.z >= minPoint2.z;

	return collideX && collideY && collideZ;
}

bool Collider::areCollided(const Model& pModel1, const Model& pModel2)
{
	glm::vec3 minPoint1 = getMinPoint(pModel1.getPos(), pModel1.getSize());
	glm::vec3 maxPoint1 = getMaxPoint(pModel1.getPos(), pModel1.getSize());
	glm::vec3 minPoint2 = getMinPoint(pModel2.getPos(), pModel2.getSize());
	glm::vec3 maxPoint2 = getMaxPoint(pModel2.getPos(), pModel2.getSize());
	
	bool collideX = minPoint1.x <= maxPoint2.x && maxPoint1.x >= minPoint2.x;
	bool collideY = minPoint1.y <= maxPoint2.y && maxPoint1.y >= minPoint2.y;
	bool collideZ = minPoint1.z <= maxPoint2.z && maxPoint1.z >= minPoint2.z;

	return collideX && collideY && collideZ;
}

glm::vec3 Collider::getMinPoint(const glm::vec3& pPos, const glm::vec3& pSize)
{
	return pPos - pSize * 0.5f;
}

glm::vec3 Collider::getMaxPoint(const glm::vec3& pPos, const glm::vec3& pSize)
{
	return pPos + pSize * 0.5f;
}
