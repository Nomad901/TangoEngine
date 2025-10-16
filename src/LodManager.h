#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "glm/glm.hpp"

class LodManager
{
public:
	struct PatchLod
	{
		int32_t mCore{ 0 };
		int32_t mLeft{ 0 };
		int32_t mRight{ 0 };
		int32_t mTop{ 0 };
		int32_t mBottom{ 0 };
	};
public:
	LodManager() = default;
	LodManager(uint32_t pPatchSize, uint32_t pNumPatchesX, uint32_t pNumPatchesZ, float pDistanceOfPatches, float pWorldScale);

	int32_t initLodManger(uint32_t pPatchSize, uint32_t pNumPatchesX, uint32_t pNumPatchesZ, float pDistanceOfPatches, float pWorldScale);

	const PatchLod& getPatchLod(int32_t pPatchX, int32_t pPatchZ) const noexcept;
	void printLodMap();

	void update(const glm::vec3& pCameraPos);

private:
	void calcLodRegions(float pDistanceOfPatches);
	void calcMaxLOD();
	void updateLodMapPass1(const glm::vec3& pCameraPos);
	void updateLodMapPass2(const glm::vec3& pCameraPos);

	int32_t getDistanceToLod(float pDistance);
private:
	uint32_t mMaxLod{ 0 };
	uint32_t mPatchSize{ 0 };
	uint32_t mNumPatchesX{ 0 }, mNumPatchesZ{ 0 };
	
	float mWorldScale{ 0.0f };

	std::vector<std::vector<PatchLod>> mPatchesLod;
	std::vector<int32_t> mRegions;
};

