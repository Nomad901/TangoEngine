#include "Volume.h"

bool Sphere::isOnFrustum(const FrustumCulling& pFrustumCulling, const glm::mat4& pModelMatrix)
{
	const glm::vec3 globalScale = glm::vec3(glm::length(glm::vec3(pModelMatrix[0][0], pModelMatrix[0][1], pModelMatrix[0][2])),
											glm::length(glm::vec3(pModelMatrix[1][0], pModelMatrix[1][1], pModelMatrix[1][2])),
											glm::length(glm::vec3(pModelMatrix[2][0], pModelMatrix[2][1], pModelMatrix[2][2])));
	const glm::vec3 globalCenter{ pModelMatrix * glm::vec4(mCenter, 1.0f) };
	const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

	//Sphere globalSphere(globalCenter, )
}
