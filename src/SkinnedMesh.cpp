#include "SkinnedMesh.h"

SkinnedMesh::~SkinnedMesh()
{
	destroy();
}

void SkinnedMesh::loadMesh(const std::filesystem::path& pPath)
{
	mTimer.setDimensionOfTime(Dimension::NANOSECONDS);
	destroy();
	
	mVAO.generate();
	mVAO.bind();
	
	std::array<uint32_t, static_cast<uint32_t>(BUFFER_TYPE::NUM_TYPE_BUFFERS)> buffers;
	glGenBuffers(mBuffers.size(), buffers.data());
	for (size_t i = 0; i < mBuffers.size(); ++i)
	{
		mBuffers[i].setID(buffers[i]);
	}

	const uint32_t ASSIMP_LOAD_FLAGS = (aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
	mScene = mImporter.ReadFile(pPath.string(), ASSIMP_LOAD_FLAGS);

	if (mScene)
	{
		mGlobalInverseTransf = Utils::getInstance().getGlmMatrix4FromAiMat4x4(mScene->mRootNode->mTransformation);
		mGlobalInverseTransf = glm::inverse(mGlobalInverseTransf);
		initFromSceneAssimp(mScene, pPath);
	}
	else
		std::cout << std::format("Couldnt load the path: {}\n", pPath.string());
	
	mVAO.unbind();
}

void SkinnedMesh::destroy()
{
	mVAO.destroy();
	for (auto& i : mBuffers)
	{
		i.destroy();
	}
}

uint32_t SkinnedMesh::getNumBones() const
{
	return mBonesInfo.size();
}

uint32_t SkinnedMesh::getNumVertices() const
{
	return mPos.size();
}

uint32_t SkinnedMesh::getNumIndices() const
{
	return mIndices.size();
}

Transform& SkinnedMesh::getTransform() noexcept
{
	return mTransform;
}

PBRMaterial& SkinnedMesh::getMaterial() noexcept
{
	for (auto& i : mMaterials)
	{
		if (i.mAmbientColor != glm::vec4(0.0f, 0.0f, 0.0f, 0.0f))
			return i;
	}
	return mMaterials[0];
}

void SkinnedMesh::getBoneTransformations(float pAnimTime, std::vector<glm::mat4>& pTransformations)
{
	glm::mat4 identityMatrix = glm::mat4(1.0f);

	float ticksPerSecond = static_cast<float>(mScene->mAnimations[0]->mTicksPerSecond != 0 ? mScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float timeInTick = pAnimTime * ticksPerSecond;
	float animationsTimeTick = fmod(timeInTick, static_cast<float>(mScene->mAnimations[0]->mDuration));

	readNodeHierachy(animationsTimeTick, mScene->mRootNode, identityMatrix);
	pTransformations.resize(mBonesInfo.size());

	for (size_t i = 0; i < mBonesInfo.size(); ++i)
	{
		pTransformations[i] = mBonesInfo[i].mTransformation;
	}
}

void SkinnedMesh::render()
{
	mVAO.bind();

	for (size_t i = 0; i < mMeshes.size(); ++i)
	{
		uint32_t materialIndex = mMeshes[i].mMaterialIndex;
		if (materialIndex > mMaterials.size())
		{
			std::cout << std::format("Material index: {}, is bigger than the size of materials: {}!\n", materialIndex, mMaterials.size());
			return;
		}

		uint32_t typeBaseIndex = mMaterials[materialIndex].getIndex(PBRMaterial::TEXTURE_TYPE::TEX_TYPE_BASE);
		uint32_t typeSpecIndex = mMaterials[materialIndex].getIndex(PBRMaterial::TEXTURE_TYPE::TEX_TYPE_SPECULAR);

		if (mMaterials[materialIndex].mTextures[typeBaseIndex])
			mMaterials[materialIndex].mTextures[typeBaseIndex]->bind(0);
		if (mMaterials[materialIndex].mTextures[typeSpecIndex])
			mMaterials[materialIndex].mTextures[typeSpecIndex]->bind(1);
		
		glDrawElementsBaseVertex(GL_TRIANGLES, mMeshes[i].mNumIndices, GL_UNSIGNED_INT,
								(void*)(sizeof(uint32_t) * mMeshes[i].mBaseIndex),
								mMeshes[i].mBaseVertex);
	}
}

void SkinnedMesh::initFromSceneAssimp(const aiScene* pScene, const std::filesystem::path& pPath)
{
	mMeshes.resize(pScene->mNumMeshes);
	mMaterials.resize(pScene->mNumMaterials);

	auto numberVerticesAndIndices = getNumVerticesAndIndices(pScene);
	uint32_t numVertices  = numberVerticesAndIndices.first;
	uint32_t numIndices = numberVerticesAndIndices.second;

	mPos.reserve(numVertices);
	mNormals.reserve(numVertices);
	mTexCoord.reserve(numVertices);
	mIndices.reserve(numIndices);
	mBones.resize(numVertices);

	initAllMeshes(pScene);
	initMaterials(pScene, pPath);
	populateBuffers();
}

void SkinnedMesh::populateBuffers()
{
	mVAO.bind();

	//
	// was made for simplicity when calling buffers from mBuffers;
	// 
	uint32_t posBufferIndex		 = getIndexBufferType(BUFFER_TYPE::POS_BUFFER);
	uint32_t texCoordBufferIndex = getIndexBufferType(BUFFER_TYPE::TEXCOORD_BUFFER);
	uint32_t normalBufferIndex   = getIndexBufferType(BUFFER_TYPE::NORMAL_BUFFER);
	uint32_t bonesBufferIndex    = getIndexBufferType(BUFFER_TYPE::BONES_BUFFER);
	uint32_t indicesBufferIndex	 = getIndexBufferType(BUFFER_TYPE::INDEX_BUFFER);

	//
	// "defines" for convenience;
	// locations of pointers for each buffer;
	// 
	uint32_t positionLocation    = 0;
	uint32_t texCoordLocation    = 1;
	uint32_t normalLocation	     = 2;
	uint32_t boneIdLocation      = 3;
	uint32_t boneId2Location	 = 4;
	uint32_t boneWeightLocation  = 5;
	uint32_t boneWeight2Location = 6;

	//
	// was made in order to eliminate copy-paste code
	//
	auto setPointer = [](uint32_t pLocation, uint32_t pStride)
		{
			glEnableVertexAttribArray(pLocation);
			glVertexAttribPointer(pLocation, pStride, GL_FLOAT, GL_FALSE, 0, 0);
		};
	
	// 
	// positions buffer;
	//
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[posBufferIndex].getID());
	glBufferData(GL_ARRAY_BUFFER, sizeof(mPos[0]) * mPos.size(), mPos.data(), GL_STATIC_DRAW);
	setPointer(positionLocation, 3);

	// 
	// texture coordinates buffer;
	//
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[texCoordBufferIndex].getID());
	glBufferData(GL_ARRAY_BUFFER, sizeof(mTexCoord[0]) * mTexCoord.size(), mTexCoord.data(), GL_STATIC_DRAW);
	setPointer(texCoordLocation, 2);

	//
	// normals buffer;
	//
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[normalBufferIndex].getID());
	glBufferData(GL_ARRAY_BUFFER, sizeof(mNormals[0]) * mNormals.size(), mNormals.data(), GL_STATIC_DRAW);
	setPointer(normalLocation, 3);

	// 
	// bones ID buffer;
	//
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[bonesBufferIndex].getID());
	glBufferData(GL_ARRAY_BUFFER, sizeof(mBones[0]) * mBones.size(), mBones.data(), GL_STATIC_DRAW);
	// bone ID pointer
	glEnableVertexAttribArray(boneIdLocation);
	glVertexAttribIPointer(boneIdLocation, 4, GL_INT, sizeof(VertexBoneData), 0);
	glEnableVertexAttribArray(boneId2Location);
	glVertexAttribIPointer(boneId2Location, 4, GL_INT, sizeof(VertexBoneData), (const void*)(4 * sizeof(int32_t)));

	// bone weights pointer;
	glEnableVertexAttribArray(boneWeightLocation);
	glVertexAttribPointer(boneWeightLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),
						 (const void*)(MAX_NUMBER_BONES_PER_VERTEX * sizeof(int32_t)));
	glEnableVertexAttribArray(boneWeight2Location);
	glVertexAttribPointer(boneWeight2Location, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),
						 (const void*)(MAX_NUMBER_BONES_PER_VERTEX * sizeof(int32_t) + 4 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[indicesBufferIndex].getID());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices[0]) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);
}

std::pair<uint32_t, uint32_t> SkinnedMesh::getNumVerticesAndIndices(const aiScene* pScene)
{
	uint32_t numberVertices = 0;
	uint32_t numberIndices = 0;

	for (size_t i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].mBaseIndex     = numberIndices;
		mMeshes[i].mBaseVertex    = numberVertices;
		mMeshes[i].mMaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		mMeshes[i].mNumIndices    = pScene->mMeshes[i]->mNumFaces * 3;

		numberVertices += pScene->mMeshes[i]->mNumVertices;
		numberIndices += mMeshes[i].mNumIndices;
	}

	return std::make_pair(numberVertices, numberIndices);
}

void SkinnedMesh::initAllMeshes(const aiScene* pScene)
{
	for (size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = pScene->mMeshes[i];
		initSingleMesh(i, mesh);
	}
}

void SkinnedMesh::initSingleMesh(uint32_t pIndex, const aiMesh* pMesh)
{
	glm::vec3 zeroVec = glm::vec3(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < pMesh->mNumVertices; ++i)
	{
		const aiVector3D& pos = pMesh->mVertices[i];
		mPos.push_back(glm::vec3(pos.x, pos.y, pos.z));

		const aiVector3D& normals = pMesh->mNormals ? pMesh->mNormals[i] : aiVector3D(0.0f, 1.0f, 0.0f);
		mNormals.push_back(glm::vec3(normals.x, normals.y, normals.z));

		const aiVector3D& texCoord = pMesh->HasTextureCoords(0) ? pMesh->mTextureCoords[0][i] : aiVector3D(zeroVec.x,
																										   zeroVec.y, 
																										   zeroVec.z);
		mTexCoord.push_back(glm::vec2(texCoord.x, texCoord.y));
	}

	loadMeshBones(pIndex, pMesh);

	for (size_t i = 0; i < pMesh->mNumFaces; ++i)
	{
		const aiFace& face = pMesh->mFaces[i];

		mIndices.push_back(face.mIndices[0]);
		mIndices.push_back(face.mIndices[1]);
		mIndices.push_back(face.mIndices[2]);
	}
}

void SkinnedMesh::initMaterials(const aiScene* pScene, const std::filesystem::path& pPath)
{
	std::string directory = Utils::getInstance().getDirectoryFromFilePath(pPath);
	for (size_t i = 0; i < pScene->mNumMaterials; ++i)
	{
		const aiMaterial* material = pScene->mMaterials[i];
		loadTexture(directory, material, i);
		loadColors(material, i);
	}
}

void SkinnedMesh::loadTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex)
{
	loadDiffuseTexture(pPath, pMaterial, pIndex);
	loadSpecularTexture(pPath, pMaterial, pIndex);
}

void SkinnedMesh::loadDiffuseTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex)
{
	uint32_t indexTexType = mMaterials[pIndex].getIndex(PBRMaterial::TEXTURE_TYPE::TEX_TYPE_BASE);
	mMaterials[pIndex].mTextures[indexTexType] = nullptr;
	
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString materialPath;

		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &materialPath) == AI_SUCCESS)
		{
			std::string fullPath = pPath.string() + '/' + materialPath.data;
			
			mMaterials[pIndex].mTextures[indexTexType] = std::make_unique<Texture2>(fullPath);
			mMaterials[pIndex].mTextures[indexTexType]->setTarget(GL_TEXTURE_2D);
		}
	}
}

void SkinnedMesh::loadSpecularTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex)
{
	uint32_t indexTexType = mMaterials[pIndex].getIndex(PBRMaterial::TEXTURE_TYPE::TEX_TYPE_SPECULAR);
	mMaterials[pIndex].mTextures[indexTexType] = nullptr;

	if (pMaterial->GetTextureCount(aiTextureType_SHININESS))
	{
		aiString materialPath;

		if (pMaterial->GetTexture(aiTextureType_SHININESS, 0, &materialPath) == AI_SUCCESS)
		{
			std::string fullPath = pPath.string() + '/' + materialPath.data;

			mMaterials[pIndex].mTextures[indexTexType] = std::make_unique<Texture2>(fullPath);
			mMaterials[pIndex].mTextures[indexTexType]->setTarget(GL_TEXTURE_2D);
		}
	}
}

void SkinnedMesh::loadColors(const aiMaterial* pMaterial, uint32_t pIndex)
{
	aiColor4D ambientColor = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	aiColor4D diffuseColor = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	aiColor4D specularColor = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);

	glm::vec4 defaultAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);  
	glm::vec4 defaultDiffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);  
	glm::vec4 defaultSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);  

	if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS) 
		mMaterials[pIndex].mAmbientColor = glm::vec4(ambientColor.r, ambientColor.g, ambientColor.b, 1.0f);
	else 
		mMaterials[pIndex].mAmbientColor = defaultAmbient;

	if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS) 
		mMaterials[pIndex].mDiffuseColor = glm::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0f);
	else 
		mMaterials[pIndex].mDiffuseColor = defaultDiffuse;

	if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS) 
		mMaterials[pIndex].mSpecularColor = glm::vec4(specularColor.r, specularColor.g, specularColor.b, 1.0f);
	else 
		mMaterials[pIndex].mSpecularColor = defaultSpecular;
}

void SkinnedMesh::loadMeshBones(uint32_t pIndex, const aiMesh* pMesh)
{
	for (size_t i = 0; i < pMesh->mNumBones; ++i)
	{
		loadSingleBone(pIndex, pMesh->mBones[i]);
	}
}

void SkinnedMesh::loadSingleBone(uint32_t pIndex, const aiBone* pBone)
{
	uint32_t boneId = getBonesIndex(pBone);

	if (boneId == mBonesInfo.size())
	{
		boneInfo tmpBoneInfo(Utils::getInstance().getGlmMatrix4FromAiMat4x4(pBone->mOffsetMatrix));
		mBonesInfo.push_back(tmpBoneInfo);
	}

	for (size_t i = 0; i < pBone->mNumWeights; ++i)
	{
		const aiVertexWeight& vertexWeight = pBone->mWeights[i];
		uint32_t globalVertexID = mMeshes[pIndex].mBaseVertex + pBone->mWeights[i].mVertexId;
		mBones[globalVertexID].addBoneData(boneId, vertexWeight.mWeight);
	}
}

int32_t SkinnedMesh::getBonesIndex(const aiBone* pBone)
{
	int32_t boneId = 0;
	std::string boneName = pBone->mName.C_Str();

	if (!mBonesIndices.contains(boneName))
	{
		boneId = mBonesIndices.size();
		mBonesIndices.emplace(boneName, boneId);
	}
	else
	{
		boneId = mBonesIndices[boneName];
	}

	return boneId;
}

uint32_t SkinnedMesh::getIndexBufferType(BUFFER_TYPE pBUFFER_TYPE)
{
	return static_cast<uint32_t>(pBUFFER_TYPE);
}

void SkinnedMesh::readNodeHierachy(float pAnimTime, const aiNode* pNode, const glm::mat4& pTransformation)
{
	std::string nodeName = pNode->mName.data;
	glm::mat4 nodeTransformation = Utils::getInstance().getGlmMatrix4FromAiMat4x4(pNode->mTransformation);

	const aiAnimation* animation = mScene->mAnimations[0]; 
	const aiNodeAnim* nodeAnimation = findNodeAnim(animation, nodeName);

	if (nodeAnimation)
	{
		aiVector3D scaling;
		calcInterpolatedScale(scaling, pAnimTime, nodeAnimation);
		glm::mat4 scaleMatrix = glm::mat4(1.0f);
		scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scaling.x, scaling.y, scaling.z));
		
		aiVector3D position;
		calcInterpolatedPosition(position, pAnimTime, nodeAnimation);
		glm::mat4 translationMatrix = glm::mat4(1.0f);
		translationMatrix = glm::translate(translationMatrix, glm::vec3(position.x, position.y, position.z));
		
		aiQuaternion rotation;
		calcInterpolatedRotation(rotation, pAnimTime, nodeAnimation);
		glm::mat4 rotationMatrix = Utils::getInstance().getGlmMatrix4FromAiMat4x4(static_cast<aiMatrix4x4>(rotation.GetMatrix()));
		
		nodeTransformation = translationMatrix * scaleMatrix * rotationMatrix;
	}

	glm::mat4 globalTransformation = pTransformation * nodeTransformation;

	if (mBonesIndices.contains(nodeName))
	{
		uint32_t boneIndex = mBonesIndices[nodeName];
		mBonesInfo[boneIndex].mTransformation = mGlobalInverseTransf * globalTransformation * mBonesInfo[boneIndex].mOffset;
	}

	for (size_t i = 0; i < pNode->mNumChildren; ++i)
	{
		readNodeHierachy(pAnimTime, pNode->mChildren[i], globalTransformation);
	} 
}

const aiNodeAnim* SkinnedMesh::findNodeAnim(const aiAnimation* pAnimation, std::string_view pNodeName)
{
	std::string nodeAnimName = std::string(pNodeName);
	if (mStorageNodesAnim.contains(nodeAnimName))
	{
		return mStorageNodesAnim[nodeAnimName];
	}

	for (size_t i = 0; i < pAnimation->mNumChannels; ++i)
	{
		const aiNodeAnim* animMesh = pAnimation->mChannels[i];
		if (std::string(animMesh->mNodeName.data) == pNodeName)
		{
			mStorageNodesAnim.insert_or_assign(nodeAnimName, animMesh);
			return animMesh;
		}
	}

	return nullptr;
}

void SkinnedMesh::calcInterpolatedScale(aiVector3D& pScaling, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim)
{
	if (pAiNodeAnim->mNumScalingKeys == 1)
	{
		pScaling = pAiNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	uint32_t scalingIndex = findInterpolatedIndex(IndexType::SCALING_INDEX, pAnimTimeTicks, pAiNodeAnim);
	uint32_t nextScalingIndex = scalingIndex + 1;

	assert(nextScalingIndex < pAiNodeAnim->mNumScalingKeys);

	float timeCurrentScalingIndex = static_cast<float>(pAiNodeAnim->mScalingKeys[scalingIndex].mTime);
	float timeNextScalingIndex	  = static_cast<float>(pAiNodeAnim->mScalingKeys[nextScalingIndex].mTime);
	float deltaTime = timeNextScalingIndex - timeCurrentScalingIndex;
	float factorTime = (pAnimTimeTicks - timeCurrentScalingIndex) / deltaTime;
	
	assert(factorTime >= 0.0f && factorTime <= 1.0f);

	const aiVector3D& startAnimation = pAiNodeAnim->mScalingKeys[scalingIndex].mValue;
	const aiVector3D& endAnimation = pAiNodeAnim->mScalingKeys[nextScalingIndex].mValue;
	aiVector3D deltaStartEndAnim = endAnimation - startAnimation;

	pScaling = startAnimation + factorTime * deltaStartEndAnim; 
}

void SkinnedMesh::calcInterpolatedRotation(aiQuaternion& pRotation, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim)
{
	if (pAiNodeAnim->mNumRotationKeys == 1) {
		pRotation = pAiNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	uint32_t rotationIndex = findInterpolatedIndex(IndexType::ROTATION_INDEX, pAnimTimeTicks, pAiNodeAnim);
	uint32_t nextRotationIndex = rotationIndex + 1;

	assert(nextRotationIndex < pAiNodeAnim->mNumRotationKeys);

	float timeCurrentRotationIndex = static_cast<float>(pAiNodeAnim->mRotationKeys[rotationIndex].mTime);
	float timeNextRotationIndex	   = static_cast<float>(pAiNodeAnim->mRotationKeys[nextRotationIndex].mTime);
	float deltaTime = timeNextRotationIndex - timeCurrentRotationIndex;
	float factorTime = (pAnimTimeTicks - timeCurrentRotationIndex) / deltaTime;
	factorTime = glm::clamp(factorTime, 0.0f, 1.0f);

	const aiQuaternion& start = pAiNodeAnim->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& end = pAiNodeAnim->mRotationKeys[nextRotationIndex].mValue;

	aiQuaternion::Interpolate(pRotation, start, end, factorTime);
	pRotation = pRotation.Normalize();
}

void SkinnedMesh::calcInterpolatedPosition(aiVector3D& pPosition, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim)
{
	if (pAiNodeAnim->mNumPositionKeys == 1)
	{
		pPosition = pAiNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	uint32_t positionIndex = findInterpolatedIndex(IndexType::POSITION_INDEX, pAnimTimeTicks, pAiNodeAnim);
	uint32_t nextPositionIndex = positionIndex + 1;

	assert(nextPositionIndex < pAiNodeAnim->mNumPositionKeys);

	float timeCurrentPositionIndex = static_cast<float>(pAiNodeAnim->mPositionKeys[positionIndex].mTime);
	float timeNextPositionIndex    = static_cast<float>(pAiNodeAnim->mPositionKeys[nextPositionIndex].mTime);
	float deltaTime = timeNextPositionIndex - timeCurrentPositionIndex;
	float factorTime = (pAnimTimeTicks - timeCurrentPositionIndex) / deltaTime;

	assert(factorTime >= 0.0f && factorTime <= 1.0f);

	const aiVector3D& startAnimation = pAiNodeAnim->mPositionKeys[positionIndex].mValue;
	const aiVector3D& endAnimation = pAiNodeAnim->mPositionKeys[nextPositionIndex].mValue;
	aiVector3D deltaStartEndAnim = endAnimation - startAnimation;

	pPosition = startAnimation + factorTime * deltaStartEndAnim;
}

uint32_t SkinnedMesh::findInterpolatedIndex(IndexType pIndexType, float pAnimTicks, const aiNodeAnim* pAiNodeAnim)
{
	uint32_t numKeys = 0;

	switch (pIndexType)
	{
	case SkinnedMesh::IndexType::SCALING_INDEX:
		numKeys = pAiNodeAnim->mNumScalingKeys;
		break;
	case SkinnedMesh::IndexType::ROTATION_INDEX:
		numKeys = pAiNodeAnim->mNumRotationKeys;
		break;
	case SkinnedMesh::IndexType::POSITION_INDEX:
		numKeys = pAiNodeAnim->mNumPositionKeys;
		break;
	}
	
	if (numKeys <= 1)
		return 0;

	auto getNodeAnimTime = [&](IndexType pIndexType, uint32_t pIndex) -> float
		{
			switch (pIndexType)
			{
			case SkinnedMesh::IndexType::SCALING_INDEX:
				return pAiNodeAnim->mScalingKeys[pIndex].mTime;
			case SkinnedMesh::IndexType::ROTATION_INDEX:
				return pAiNodeAnim->mRotationKeys[pIndex].mTime;
			case SkinnedMesh::IndexType::POSITION_INDEX:
				return pAiNodeAnim->mPositionKeys[pIndex].mTime;
			}
			return 0.0f;
		};

	float firstKeyTime  = getNodeAnimTime(pIndexType, 0);
	float lastKeyTime = getNodeAnimTime(pIndexType, numKeys - 1);

	if (pAnimTicks <= firstKeyTime)
		return 0;
	if (pAnimTicks >= lastKeyTime)
		return numKeys - 2;

	uint32_t low = 0;
	uint32_t high = numKeys - 1;
	uint32_t result = 0;

	while (low <= high)
	{
		uint32_t mid = low + (high - low) / 2;
		float midKeyTime = getNodeAnimTime(pIndexType, mid);

		if (midKeyTime <= pAnimTicks)
		{
			float nextTime = getNodeAnimTime(pIndexType, mid + 1);
			if (pAnimTicks < nextTime)
			{
				result = mid;
				break;
			}

			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}

	return result;
}

void SkinnedMesh::VertexBoneData::addBoneData(uint32_t pBoneId, float pBoneWeight)
{
	uint32_t counter = 0;
	for (size_t i = 0; i < mBonesId.size(); ++i)
	{
		if (mWeights[i] == 0.0f)
		{
			mBonesId[i] = pBoneId;
			mWeights[i] = pBoneWeight;
			return;
		}
		counter++;
	}
	std::cout << std::format("We have more bones than we can dissect: {}!\n", counter);
}
