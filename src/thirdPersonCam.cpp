#include "thirdPersonCam.h"

void thirdPersonCam::update(const SDL_Event& pEvents, const glm::vec3& pCharacterPos, float pRotationCharY)
{
	calculateDistance(pEvents);
	calculateMouseActions(pEvents);
	
	float verticalDistance = mDistance * glm::sin(glm::radians(Camera::getPitch()));
	float horizontalDistance = mDistance * glm::cos(glm::radians(Camera::getPitch()));

	calculateCameraPosition(verticalDistance, horizontalDistance, pCharacterPos, pRotationCharY);

	Camera::setYaw(180.0f - (pRotationCharY - mAngleAroundPlayer));
}

void thirdPersonCam::calculateDistance(const SDL_Event& pEvents)
{
	if (pEvents.type == SDL_EVENT_MOUSE_WHEEL)
	{ 
		float distanceLevel = pEvents.wheel.y * 0.1f;
		mDistance -= distanceLevel;
	}
}

void thirdPersonCam::calculateMouseActions(const SDL_Event& pEvents)
{
	if (pEvents.type == SDL_EVENT_MOUSE_MOTION)
	{
		if (pEvents.motion.state & SDL_BUTTON_RMASK)
		{
			float pitchChange = -pEvents.motion.yrel * 0.1f;
			float newPitch = Camera::getPitch() + pitchChange;
			newPitch = glm::clamp(newPitch, -89.0f, 89.0f);
			Camera::setPitch(newPitch);
		}
		if (pEvents.motion.state & SDL_BUTTON_LMASK)
		{
			float angleChange = pEvents.motion.xrel * 0.3f;
			mAngleAroundPlayer += angleChange;
		}
	}
}

void thirdPersonCam::calculateCameraPosition(float pVerticalDistance, float pHorizontalDistance,
											 const glm::vec3& pCharacterPos, float pRotationCharY)
{
	float theta = pRotationCharY + mAngleAroundPlayer;
	float offsetX = pHorizontalDistance * glm::sin(glm::radians(theta));
	float offsetZ = pHorizontalDistance * glm::cos(glm::radians(theta));

	glm::vec3 cameraPos; 
	cameraPos.x = pCharacterPos.x - offsetX;
	cameraPos.y = pCharacterPos.y + pVerticalDistance + 5.0f;
	cameraPos.z = pCharacterPos.z - offsetZ;
	Camera::setPos(cameraPos);
	
	glm::vec3 lookAtTarget = pCharacterPos + glm::vec3(0.0f, 5.0f, 0.0f);
	Camera::setDirection(glm::normalize(lookAtTarget - cameraPos));
}
