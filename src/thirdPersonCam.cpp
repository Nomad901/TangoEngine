#include "thirdPersonCam.h"

void thirdPersonCam::update(SDL_Event& pEvents, const glm::vec3& pCharacterPos)
{
	calculateDistance(pEvents);
	calculateMouseActions(pEvents);
	
	float verticalDistance = mDistance * glm::sin(glm::radians(Camera::getPitch()));
	float horizontalDistance = mDistance * glm::cos(glm::radians(Camera::getPitch()));

	calculateCameraPosition(verticalDistance, horizontalDistance, pCharacterPos);
}

void thirdPersonCam::calculateDistance(SDL_Event& pEvents)
{
	if (pEvents.type == SDL_EVENT_MOUSE_WHEEL)
	{ 
		float distanceLevel = pEvents.wheel.y * 0.1f;
		mDistance -= distanceLevel;
	}
}

void thirdPersonCam::calculateMouseActions(SDL_Event& pEvents)
{
	if (pEvents.type == SDL_EVENT_MOUSE_MOTION)
	{
		if (pEvents.motion.state & SDL_BUTTON_RMASK)
		{
			float pitchChange = -pEvents.motion.yrel * 0.1f;
			Camera::setPitch(Camera::getPitch() - pitchChange);
		}
		if (pEvents.motion.state & SDL_BUTTON_LMASK)
		{
			float angleChange = pEvents.motion.xrel * 0.3f;
			mAngleAroundPlayer -= angleChange;
		}
	}
}

void thirdPersonCam::calculateCameraPosition(float pVerticalDistance, float pHorizontalDistance,
											 const glm::vec3& pCharacterPos)
{
	// getYaw - level of rotation of player by y axis
	float theta = Camera::getYaw() + mAngleAroundPlayer;
	float offsetX = pHorizontalDistance * glm::sin(glm::radians(theta));
	float offsetZ = pHorizontalDistance * glm::cos(glm::radians(theta));

	glm::vec3 cameraPos; 
	cameraPos.x = pCharacterPos.x - offsetX;
	cameraPos.y = pCharacterPos.y + pVerticalDistance;
	cameraPos.z = pCharacterPos.z - offsetZ;
	Camera::setPos(cameraPos);
}
