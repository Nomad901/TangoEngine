#pragma once
#include <iostream>

#include "glm.hpp"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "Camera.h"
#include "Mesh.h"

class MousePicker
{
public:
	MousePicker() = default;
	MousePicker(const Camera& pCamera, const glm::mat4& pProjMat, const glm::vec2& pWinSize);

	void init(const Camera& pCamera, const glm::mat4& pProjMat, const glm::vec2& pWinSize);

	glm::vec3 getPosRay() const noexcept;

	bool checkIntersection(const Mesh& pMesh);

	void update(Camera& pCamera, const glm::vec2& pMousePos);

private:
	glm::vec3 calculateMouseRay(const glm::vec2& pMousePos);

	glm::vec3 getWorldCoords(const glm::vec4& pEyeCoords);
	glm::vec4 getEyeCoord(const glm::vec4& pClipCoord);
	glm::vec2 getNormalizedDeviceCoord(const glm::vec2& pMousePos);

	std::optional<float> intersectRayAABB(const glm::vec3& pRayOrigin, const glm::vec3& pRayDir, 
										  const glm::vec3& pMinBounds, const glm::vec3& pMaxBounds);

private:
	glm::vec3 mPosRay;
	glm::mat4 mProjMat;

	glm::vec2 mWinSize;

	Camera mCamera;
};

