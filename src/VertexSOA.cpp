#include "VertexSOA.h"

VertexSOA::VertexSOA(uint32_t pCapacity)
{
	reserveSpace(pCapacity);
}

void VertexSOA::addVertex(glm::vec3&& pPos, glm::vec3&& pNormal, glm::vec3&& pTexCoords)
{
	mPosX.push_back(pPos.x);
	mPosY.push_back(pPos.y);
	mPosZ.push_back(pPos.z);

	mNormalX.push_back(pNormal.x);
	mNormalY.push_back(pNormal.y);
	mNormalZ.push_back(pNormal.z);

	mTexCoordX.push_back(pTexCoords.x);
	mTexCoordY.push_back(pTexCoords.y);

	mInitializationState = InitializationState::WITH_NOTHING;

	mNumberOfVertices++;
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

	mInitializationState = InitializationState::WITH_COLORS;

	mNumberOfVertices++;
}

void VertexSOA::addVertex(glm::vec3&& pPos, glm::vec3&& pNormal,
						  glm::vec2&& pTexCoords, glm::vec3&& pTangent, glm::vec3&& pBitangent)
{
	mPosX.push_back(pPos.x);
	mPosY.push_back(pPos.y);
	mPosZ.push_back(pPos.z);

	mNormalX.push_back(pNormal.x);
	mNormalY.push_back(pNormal.y);
	mNormalZ.push_back(pNormal.z);

	mTexCoordX.push_back(pTexCoords.x);
	mTexCoordY.push_back(pTexCoords.y);

	mTangentX.push_back(pTangent.x);
	mTangentY.push_back(pTangent.y);
	mTangentZ.push_back(pTangent.z);

	mBitangentX.push_back(pBitangent.x);
	mBitangentY.push_back(pBitangent.y);
	mBitangentZ.push_back(pBitangent.z);

	mInitializationState = InitializationState::WITH_TANGENT;

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

void VertexSOA::updateVertex(size_t pIndex, glm::vec3&& pPos, glm::vec3&& pNormal,
											glm::vec2&& pTexCoord, glm::vec3&& pTangent, 
											glm::vec3&& pBitangent)
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

	mTangentX[pIndex] = pTangent.x;
	mTangentY[pIndex] = pTangent.y;
	mTangentZ[pIndex] = pTangent.z;
	
	mBitangentX[pIndex] = pBitangent.x;
	mBitangentY[pIndex] = pBitangent.y;
	mBitangentZ[pIndex] = pBitangent.z;
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

auto VertexSOA::getColorsX() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_COLORS)
		return std::cref(mColorX);
	return std::nullopt;
}

auto VertexSOA::getColorsY() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_COLORS)
		return std::cref(mColorY);
	return std::nullopt; 
}

auto VertexSOA::getColorsZ() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_COLORS)
		return std::cref(mColorZ);
	return std::nullopt;
}

auto VertexSOA::getColorsW() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_COLORS)
		return std::cref(mColorW);
	return std::nullopt;
}

auto VertexSOA::getTangentX() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_TANGENT)
		return std::cref(mTangentX);
	return std::nullopt;
}

auto VertexSOA::getTangentY() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_TANGENT)
		return std::cref(mTangentY);
	return std::nullopt;
}

auto VertexSOA::getTangentZ() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_TANGENT)
		return std::cref(mTangentZ);
	return std::nullopt;
}

auto VertexSOA::getBitangentX() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_TANGENT)
		return std::cref(mBitangentX);
	return std::nullopt;
}

auto VertexSOA::getBitangentY() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_TANGENT)
		return std::cref(mBitangentY);
	return std::nullopt;
}

auto VertexSOA::getBitangentZ() const noexcept -> std::optional<std::reference_wrapper<const std::vector<float>>>
{
	if (mInitializationState == InitializationState::WITH_TANGENT)
		return std::cref(mBitangentZ);
	return std::nullopt;
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

bool VertexSOA::hasColors() const noexcept
{
	return !mColorX.empty();
}

bool VertexSOA::hasTangent() const noexcept
{
	return !mTangentX.empty();
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
