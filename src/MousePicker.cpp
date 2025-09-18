#include "MousePicker.h"

MousePicker::MousePicker(const Camera& pCamera, const glm::mat4& pProjMat, const glm::vec2& pWinSize)
{
	init(pCamera, pProjMat, pWinSize);
}

void MousePicker::init(const Camera& pCamera, const glm::mat4& pProjMat, const glm::vec2& pWinSize)
{
	mCamera = pCamera;
	mProjMat = pProjMat;
	mWinSize = pWinSize;
}

glm::vec3 MousePicker::getPosRay() const noexcept
{
	return mPosRay;
}

bool MousePicker::checkIntersection(const Mesh& pMesh)
{
	const glm::vec3 rayOrigin = mCamera.getPos();
	const glm::vec3 rayDir = mPosRay;

	constexpr float closestDistance = std::numeric_limits<float>().max();
	
	const glm::vec3 minBounds = pMesh.getPos() - pMesh.getSize() / 2.0f;
	const glm::vec3 maxBounds = pMesh.getPos() + pMesh.getSize() / 2.0f;

	auto intersection = intersectRayAABB(rayOrigin, rayDir, minBounds, maxBounds);
	
	if (intersection.has_value() && intersection.value() < closestDistance)
		return true;

	return false;
}

void MousePicker::update(Camera& pCamera, const glm::vec2& pMousePos)
{
	mCamera = pCamera;
	mPosRay = calculateMouseRay(pMousePos);
}

glm::vec3 MousePicker::calculateMouseRay(const glm::vec2& pMousePos)
{
	float mouseX = pMousePos.x;
	float mouseY = pMousePos.y;
	glm::vec2 normalizedMouse = getNormalizedDeviceCoord({ mouseX, mouseY });
	glm::vec4 clipCoord = glm::vec4(normalizedMouse.x, normalizedMouse.y, -1.0f, 1.0f);
	glm::vec4 eyeCoord = getEyeCoord(clipCoord);
	glm::vec3 worldRay = getWorldCoords(eyeCoord);
	return worldRay;
}

glm::vec3 MousePicker::getWorldCoords(const glm::vec4& pEyeCoords)
{
	glm::mat4 invertedView = glm::inverse(mCamera.getViewMatrix());
	glm::vec4 rayWorld = invertedView * pEyeCoords;
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = glm::normalize(mouseRay);
	return mouseRay;
}

glm::vec4 MousePicker::getEyeCoord(const glm::vec4& pClipCoord)
{
	glm::mat4 invertedProj = glm::inverse(mProjMat);
	glm::vec4 eyeCoord = invertedProj * pClipCoord;
	return glm::vec4(eyeCoord.x, eyeCoord.y, -1.0f, 0.0f);
}

glm::vec2 MousePicker::getNormalizedDeviceCoord(const glm::vec2& pMousePos)
{
	float x = (2.0f * pMousePos.x) / mWinSize.x - 1.0f;
	float y = (2.0f * pMousePos.y) / mWinSize.y - 1.0f;
	return glm::vec2(x, y); // maybe need to invert y axis
}

std::optional<float> MousePicker::intersectRayAABB(const glm::vec3& pRayOrigin, const glm::vec3& pRayDir, 
												   const glm::vec3& pMinBounds, const glm::vec3& pMaxBounds)
{
	const glm::vec3 invDir = 1.0f / pRayDir;

	const float t1 = (pMinBounds.x - pRayOrigin.x) * invDir.x;
	const float t2 = (pMaxBounds.x - pRayOrigin.x) * invDir.x;
	const float t3 = (pMinBounds.y - pRayOrigin.y) * invDir.y;
	const float t4 = (pMaxBounds.y - pRayOrigin.y) * invDir.y;
	const float t5 = (pMinBounds.z - pRayOrigin.z) * invDir.z;
	const float t6 = (pMaxBounds.z - pRayOrigin.z) * invDir.z;
	
	const float tMin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	const float tMax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tMax < 0.0f || tMin > tMax)
		return std::nullopt;
	return tMin;
}
