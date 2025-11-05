#include "FPSRegulator.h"

void FPSRegulator::beginFrame()
{
	mBeginFrame = SDL_GetTicks();
	static uint32_t lastTime = SDL_GetTicks();
	uint32_t currentTime = SDL_GetTicks();

	mPhysicsDeltaTime = (static_cast<float>(currentTime) - static_cast<float>(lastTime));
	lastTime = currentTime;
}

void FPSRegulator::endFrame()
{
	float deltaTime = SDL_GetTicks() - mBeginFrame;
	if (deltaTime < 8)
		SDL_Delay(8 - deltaTime);
	Utils::getInstance().updateDeltaTime(deltaTime); // TODO: maybe i need to recompute the delta time, cuz ive set the delay on the program;
}

float FPSRegulator::getPhysicsDeltaTime(float pDividerTime)
{
	return mPhysicsDeltaTime / pDividerTime;
}