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
	return mPatchesLod.at(pPatchX).at(pPatchZ);
}

void LodManager::printLodMap()
{
	for (int32_t lodMapZ = mNumPatchesZ - 1; lodMapZ >= 0; --lodMapZ)
	{
		std::cout << std::format("Lod map Z: {}. ", lodMapZ);
		for (int32_t lodMapX = 0; lodMapX < mNumPatchesX; ++lodMapX)
		{
			std::cout << std::format("Core: {} ", lodMapX, mPatchesLod[lodMapX][lodMapZ].mCore);
		}
		std::cout<< '\n';
	}
}

void LodManager::update(const glm::vec3& pCameraPos)
{
	updateLodMapPass1(pCameraPos);
	updateLodMapPass2(pCameraPos);
}

void LodManager::calcLodRegions()
{
	int32_t sum = 0;

	for (uint32_t i = 0; i <= mMaxLod; ++i)
	{
		sum += (i + 1);
	}

#ifdef DEBUG
	std::cout << std::format("Sum from calcLodRegions: {}\n", sum);
#endif // DEBUG

	// 5000.0f - z_far;
	float x = 5000.0f / static_cast<float>(sum);
	int32_t temp = 0;

	for (int32_t i = 0; i <= mMaxLod; ++i)
	{
		int32_t currRange = static_cast<int32_t>(x * (i + 1));
		mRegions[i] = temp + currRange;
		temp += currRange;
	}
}

void LodManager::calcMaxLOD()
{
	int32_t numSegments = mPatchSize - 1;

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

	for (size_t lodMapZ = 0; lodMapZ < mNumPatchesZ; ++lodMapZ)
	{
		for (size_t lodMapX = 0; lodMapX < mNumPatchesX; ++lodMapX)
		{
			int32_t x = lodMapX * (mPatchSize - 1) + centerStep;
			int32_t z = lodMapZ * (mPatchSize - 1) + centerStep;
			
			glm::vec3 patchCenter = glm::vec3(x * static_cast<float>(mWorldScale),
											  0.0f, 
											  z * static_cast<float>(mWorldScale));

			float distanceToCamera = glm::length(patchCenter - pCameraPos);

			int32_t coreLod = getDistanceToLod(distanceToCamera);

			PatchLod* patchLod = &mPatchesLod[lodMapX][lodMapZ];
			patchLod->mCore = coreLod;
		}
	}
}

void LodManager::updateLodMapPass2(const glm::vec3& pCameraPos)
{
	int32_t step = mPatchSize / 2;

	for (size_t lodMapZ = 0; lodMapZ < mNumPatchesZ; ++lodMapZ)
	{
		for (size_t lodMapX = 0; lodMapX < mNumPatchesX; ++lodMapX)
		{
			int32_t coreLod = mPatchesLod[lodMapX][lodMapZ].mCore;

			int32_t indexLeft   = lodMapX;
			int32_t indexRight  = lodMapX;
			int32_t indexTop    = lodMapZ;
			int32_t indexBottom = lodMapZ;

			if (lodMapX > 0)
			{
				indexLeft--;
				
				if (mPatchesLod[indexLeft][lodMapZ].mCore > coreLod)
					mPatchesLod.at(lodMapX).at(lodMapZ).mLeft = 1;
				else
					mPatchesLod.at(lodMapX).at(lodMapZ).mLeft = 0;
			}
			if (lodMapX < mPatchSize - 1)
			{
				indexRight++;
				
				if (mPatchesLod[indexRight][lodMapZ].mCore > coreLod)
					mPatchesLod.at(lodMapX).at(lodMapZ).mRight = 1;
				else 
					mPatchesLod.at(lodMapX).at(lodMapZ).mRight = 0;
			}
			if (lodMapZ > 0)
			{
				indexBottom--;

				if (mPatchesLod[lodMapX][indexBottom].mCore > coreLod)
					mPatchesLod.at(lodMapX).at(lodMapZ).mBottom = 1;
				else
					mPatchesLod.at(lodMapX).at(lodMapZ).mBottom = 0;
			}
			if (lodMapZ < mPatchSize - 1)
			{
				indexTop++;
				
				if (mPatchesLod[lodMapX][indexTop].mCore > coreLod)
					mPatchesLod.at(lodMapX).at(lodMapZ).mTop = 1;
				else
					mPatchesLod.at(lodMapX).at(lodMapZ).mTop = 0;
			}
		}
	}
}

int32_t LodManager::getDistanceToLod(float pDistance)
{
	int32_t lod = mMaxLod;

	for (uint32_t i = 0; i <= mMaxLod; ++i)
	{
		if (pDistance < mRegions[i])
		{
			lod = i;
			break;
		}
	}

	return lod;
}
