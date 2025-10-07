#pragma once
#include <iostream>

#include "glm.hpp"

static float kinematicEquation(float pAcceleration, float pVelocity, float pPos, float pTime)
{
	return 0.5f * pAcceleration * pTime * pTime + pVelocity * pTime + pPos;
}