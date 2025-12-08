#include "VertexSOA.h"

VertexSOA::VertexSOA(uint32_t pCapacity)
{
	reserveSpace(pCapacity);
}

void VertexSOA::addVertex(glm::vec3&& pPos, glm::vec3&& pNormal,
						  glm::vec2&& pTexCoords, glm::vec4&& pColor)
{
	mPosX.push_back(pPos.x);     
	mPosY.push_back(pPos.y);
	mPosZ.push_back(pPos.z);

	mNormalX.push_back(pNormal.x);
	mNormalY.push_back(pNormal.y);
	mNormalZ.push_back(pNormal.z);

	mTexCoordX.push_back(pTexCoords.x);
	mTexCoordY.push_back(pTexCoords.y);

	mColorX.push_back(pColor.x);
	mColorY.push_back(pColor.y);
	mColorZ.push_back(pColor.z);
	mColorW.push_back(pColor.w);

	mNumberOfVertices++;
}

void VertexSOA::getVertex(size_t pIndex, glm::vec3& pPos, glm::vec3& pNormal, 
										 glm::vec2& pTexCoord, glm::vec4& pColor) const 
{
	assert(mNumberOfVertices > pIndex);

	pPos.x = mPosX[pIndex];
	pPos.y = mPosY[pIndex];
	pPos.z = mPosZ[pIndex];

	pNormal.x = mNormalX[pIndex];
	pNormal.y = mNormalY[pIndex];
	pNormal.z = mNormalZ[pIndex];

	pTexCoord.x = mTexCoordX[pIndex];
	pTexCoord.y = mTexCoordY[pIndex];
	
	pColor.x = mColorX[pIndex];
	pColor.y = mColorY[pIndex];
	pColor.z = mColorZ[pIndex];
	pColor.w = mColorW[pIndex];
}

void VertexSOA::updateVertex(size_t pIndex, glm::vec3&& pPos, glm::vec3&& pNormal,
											glm::vec2&& pTexCoord, glm::vec4&& pColor)
{
	assert(mNumberOfVertices > pIndex);

	mPosX[pIndex] = pPos.x;    
	mPosY[pIndex] = pPos.y;
	mPosZ[pIndex] = pPos.z;

	mNormalX[pIndex] = pNormal.x;
	mNormalY[pIndex] = pNormal.y;
	mNormalZ[pIndex] = pNormal.z;

	mTexCoordX[pIndex] = pTexCoord.x;
	mTexCoordY[pIndex] = pTexCoord.y;

	mColorX[pIndex] = pColor.x;
	mColorY[pIndex] = pColor.y;
	mColorZ[pIndex] = pColor.z;
	mColorW[pIndex] = pColor.w;
}

const std::vector<float>& VertexSOA::getPositionsX() const noexcept
{
	return mPosX;
}

const std::vector<float>& VertexSOA::getPositionsY() const noexcept
{
	return mPosY;
}

const std::vector<float>& VertexSOA::getPositionsZ() const noexcept
{
	return mPosZ;
}

const std::vector<float>& VertexSOA::getNormalsX() const noexcept
{
	return mNormalX;
}

const std::vector<float>& VertexSOA::getNormalsY() const noexcept
{
	return mNormalY;
}

const std::vector<float>& VertexSOA::getNormalsZ() const noexcept
{
	return mNormalZ;
}

const std::vector<float>& VertexSOA::getTexCoordsX() const noexcept
{
	return mTexCoordX;
}

const std::vector<float>& VertexSOA::getTexCoordsY() const noexcept
{
	return mTexCoordY;
}

const std::vector<float>& VertexSOA::getColorsX() const noexcept
{
	return mColorX;
}

const std::vector<float>& VertexSOA::getColorsY() const noexcept
{
	return mColorY;
}

const std::vector<float>& VertexSOA::getColorsZ() const noexcept
{
	return mColorZ;
}

const std::vector<float>& VertexSOA::getColorsW() const noexcept
{
	return mColorW;
}

size_t VertexSOA::getNumberOfVertices() const noexcept
{
	return mNumberOfVertices;
}

void VertexSOA::clearAllData()
{
	mPosX.clear();
	mPosY.clear();
	mPosZ.clear();
	
	mNormalX.clear();
	mNormalY.clear();
	mNormalZ.clear();
	
	mTexCoordX.clear();
	mTexCoordY.clear();

	mColorX.clear();
	mColorY.clear();
	mColorZ.clear();
	mColorW.clear();

	mNumberOfVertices = 0;
}

void VertexSOA::clearExactData(size_t pIndex)
{
	assert(mNumberOfVertices > pIndex);

	mPosX[pIndex] = 0.0f;
	mPosY[pIndex] = 0.0f;
	mPosZ[pIndex] = 0.0f;

	mNormalX[pIndex] = 0.0f;
	mNormalY[pIndex] = 0.0f;
	mNormalZ[pIndex] = 0.0f;

	mTexCoordX[pIndex] = 0.0f;
	mTexCoordY[pIndex] = 0.0f;

	mColorX[pIndex] = 0.0f;
	mColorY[pIndex] = 0.0f;
	mColorZ[pIndex] = 0.0f;
	mColorW[pIndex] = 0.0f;
}

bool VertexSOA::isEmpty(size_t pIndex) const noexcept
{
	assert(mNumberOfVertices > pIndex);

	return (mPosX[pIndex] == 0.0f && mPosY[pIndex] == 0.0f && mPosZ[pIndex] == 0.0f &&
			mNormalX[pIndex] == 0.0f && mNormalY[pIndex] == 0.0f && mNormalZ[pIndex] == 0.0f &&
			mTexCoordX[pIndex] == 0.0f && mTexCoordY[pIndex] == 0.0f &&
			mColorX[pIndex] == 0.0f && mColorY[pIndex] == 0.0f &&
			mColorZ[pIndex] == 0.0f && mColorW[pIndex] == 0.0f);
}

void VertexSOA::reserveSpace(uint32_t pCapacity)
{
	mPosX.reserve(pCapacity);
	mPosY.reserve(pCapacity);
	mPosZ.reserve(pCapacity);
	
	mNormalX.reserve(pCapacity);
	mNormalY.reserve(pCapacity);
	mNormalZ.reserve(pCapacity);

	mTexCoordX.reserve(pCapacity);
	mTexCoordY.reserve(pCapacity);
	
	mColorX.reserve(pCapacity);
	mColorY.reserve(pCapacity);
	mColorZ.reserve(pCapacity);
	mColorW.reserve(pCapacity);
}
