#include "LodManager.h"

LodManager::LodManager(uint32_t pPatchSize, uint32_t pNumPatchesX, uint32_t pNumPatchesZ, float pWorldScale)
{
	initLodManger(pPatchSize, pNumPatchesX, pNumPatchesZ, pWorldScale);
}

int32_t LodManager::initLodManger(uint32_t pPatchSize, uint32_t pNumPatchesX, uint32_t pNumPatchesZ, float pWorldScale)
{
	mPatchSize = pPatchSize;
	mNumPatchesX = pNumPatchesX;
	mNumPatchesZ = pNumPatchesZ;
	mWorldScale = pWorldScale;

	calcMaxLOD();

	PatchLod emptyPatchLod;
	mPatchesLod.resize(mNumPatchesX * mNumPatchesZ);
	for (size_t x = 0; x < mNumPatchesX; ++x)
	{
		mPatchesLod[x].resize(mNumPatchesZ);
		for (size_t z = 0; z < mNumPatchesZ; ++z)
		{
			mPatchesLod[x][z] = emptyPatchLod;
		}
	}

	mRegions.resize(mMaxLod + 1);
	calcLodRegions();

	return mMaxLod;
}

const LodManager::PatchLod& LodManager::getPatchLod(int32_t pPatchX, int32_t pPatchZ) const noexcept
{

}

void LodManager::printLodMap()
{

}

void LodManager::update(const glm::vec3& pCameraPos)
{
	updateLodMapPass1(pCameraPos);
	updateLodMapPass2(pCameraPos);
}

void LodManager::calcLodRegions()
{
}

void LodManager::calcMaxLOD()
{
	int32_t numSegments = mPatchSize == 0 ? 0 : mPatchSize - 1;

	auto result1 = ceilf(log2f(static_cast<float>(numSegments)));
	auto result2 = floorf(log2f(static_cast<float>(numSegments)));
	if (result1 != result2)
	{
		std::cout << std::format("The number of vertices in the patch needs to be a power of two. {} .... {}\n", result1, result2);
		return;
	}

	int32_t patchSizeLog2 = static_cast<int32_t>(log2f(static_cast<float>(numSegments)));
#ifdef DEBUG
	std::cout << std::format("log2f of patch size {} is {}\n", mPatchSize, patchSizeLog2);
#endif // DEBUG
	mMaxLod = patchSizeLog2 - 1;
}

void LodManager::updateLodMapPass1(const glm::vec3& pCameraPos)
{
	int32_t centerStep = mPatchSize / 2;

	
}

void LodManager::updateLodMapPass2(const glm::vec3& pCameraPos)
{
}

int32_t LodManager::getDistanceToLod(float pDistance)
{
	return 0;
}
