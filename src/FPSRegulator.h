#pragma once
#include <iostream>

#include <SDL3/SDL.h>

#include "Utils.h"

class FPSRegulator
{
public:
	FPSRegulator() = default;

	void beginFrame();
	void endFrame();

	float getPhysicsDeltaTime(float pDividerTime);

private:
	uint32_t mBeginFrame{};
	float mPhysicsDeltaTime{};
};

