#include "SkinnedMesh.h"

void SkinnedMesh::loadMesh(const std::filesystem::path& pPath)
{
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
		initFromSceneAssimp(mScene, pPath);
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
	return mBones.size();
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

void SkinnedMesh::getBoneTransformations(std::vector<glm::mat4>& pTransformations)
{
	if (!pTransformations.empty())
		pTransformations.clear();
	pTransformations.resize(mBonesInfo.size());

	glm::mat4 identityMatrix = glm::mat4(1.0f);

	readNodeHierachy(mScene->mRootNode, identityMatrix);

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
	uint32_t positionLocation   = 0;
	uint32_t texCoordLocation   = 1;
	uint32_t normalLocation	    = 2;
	uint32_t boneIdLocation     = 3;
	uint32_t boneWeightLocation = 4;

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
	glEnableVertexAttribArray(boneIdLocation + 1);
	glVertexAttribIPointer(boneIdLocation + 1, 2, GL_INT, sizeof(VertexBoneData), (const void*)(4 * sizeof(int32_t)));

	// bone weights pointer;
	glEnableVertexAttribArray(boneWeightLocation);
	glVertexAttribPointer(boneWeightLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),
						 (const void*)(MAX_NUMBER_BONES_PER_VERTEX * sizeof(int32_t)));
	glEnableVertexAttribArray(boneWeightLocation + 1);
	glVertexAttribPointer(boneWeightLocation + 1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),
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
	
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE))
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
	uint32_t indexTexType = mMaterials[pIndex].getIndex(PBRMaterial::TEXTURE_TYPE::TEX_TYPE_BASE);
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
	aiColor4D ambientColor  = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	aiColor4D diffuseColor  = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	aiColor4D specularColor = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec4 identityVec = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 zeroVec = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	int32_t shadingModel = 0;
	if (!pMaterial->Get(AI_MATKEY_SHADING_MODEL, shadingModel) == AI_SUCCESS)
	{
		std::cout << std::format("Couldnt get shading model! Shading model: {} Index of materials: {}\n", shadingModel, pIndex);
		return;
	}
	
	if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
	{
		mMaterials[pIndex].mAmbientColor = glm::vec4(ambientColor.r,
													 ambientColor.g,
													 ambientColor.b,
													 ambientColor.a);
	} 
	else
	{
		mMaterials[pIndex].mAmbientColor = identityVec;
	}

	if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
	{
		mMaterials[pIndex].mDiffuseColor = glm::vec4(diffuseColor.r,
													 diffuseColor.g,
													 diffuseColor.b,
													 diffuseColor.a);
	}
	else
	{
		mMaterials[pIndex].mDiffuseColor = zeroVec;
	}

	if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS)
	{
		mMaterials[pIndex].mSpecularColor = glm::vec4(specularColor.r,
													  specularColor.g,
													  specularColor.b,
													  specularColor.a);
	}
	else
	{
		mMaterials[pIndex].mSpecularColor = zeroVec;
	}
}

void SkinnedMesh::loadMeshBones(uint32_t pIndex, const aiMesh* pMesh)
{
	if (!pMesh->HasBones())
		return;

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

	if (mBonesIndices.contains(boneName))
	{
		boneId = mBonesIndices[boneName];
	}
	else
	{
		boneId = mBonesIndices.size();
		mBonesIndices.emplace(boneName, boneId);
	}

	return boneId;
}

uint32_t SkinnedMesh::getIndexBufferType(BUFFER_TYPE pBUFFER_TYPE)
{
	return static_cast<uint32_t>(pBUFFER_TYPE);
}

void SkinnedMesh::readNodeHierachy(const aiNode* pNode, const glm::mat4& pTransformation)
{
	std::string nodeName = pNode->mName.data;
	glm::mat4 nodeTransformation = Utils::getInstance().getGlmMatrix4FromAiMat4x4(pNode->mTransformation);
	glm::mat4 globalTransformation = pTransformation * nodeTransformation;

	if (!mBonesIndices.contains(nodeName))
	{
		uint32_t boneIndex = mBonesIndices[nodeName];
		mBonesInfo[boneIndex].mTransformation = globalTransformation * mBonesInfo[boneIndex].mOffset;
	}

	for (size_t i = 0; i < pNode->mNumChildren; ++i)
	{
		readNodeHierachy(pNode->mChildren[i], globalTransformation);
	}
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
