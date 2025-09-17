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
