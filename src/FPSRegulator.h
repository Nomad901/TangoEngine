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

	uint32_t getPhysicsDeltaTime();

private:
	uint32_t mBeginFrame{};
	uint32_t mPhysicsDeltaTime{};
};

