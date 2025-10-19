#pragma once
#include <SDL3/SDL.h>

#include "Camera.h"

class thirdPersonCam : public Camera 
{
public:
	thirdPersonCam() = default;

	void update(const SDL_Event& pEvents, const glm::vec3& pCharacterPos, float& pRotationCharY);
	void resetAngleAroundPlayer();

	float getAngleAroundPlayer() const noexcept;
	float getZoom() const noexcept override;

private:
	void calculateDistance(const SDL_Event& pEvents);
	void calculateMouseActions(const SDL_Event& pEvents);
	void calculateCameraPosition(float pVerticalDistance, float pHorizontalDistance,	
								 const glm::vec3& pCharacterPos);

private:
	float mDistance{ 50.0f };
	float mAngleAroundPlayer{ 0.0f };
};

