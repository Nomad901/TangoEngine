#include "UBO.h"

UBO::UBO(const std::vector<std::pair<uint32_t, std::string_view>>& pUniform,
		 uint32_t pIndex, size_t pSize)
{
	init(pUniform, pIndex, pSize);
}

UBO::~UBO()
{
	if (mUBOid)
		glDeleteBuffers(1, &mUBOid);
}

void UBO::init(const std::vector<std::pair<uint32_t, std::string_view>>& pUniform,
			   uint32_t pIndex, size_t pSize)
{
	for (auto& [key, value] : pUniform)
	{
		std::string convertedStr = std::string(value);

		uint32_t uniformIndex = glGetUniformBlockIndex(key, convertedStr.c_str());

		if (uniformIndex == GL_INVALID_INDEX)
		{
			std::cout << std::format("ERROR! The index is incorrect. Maybe an issue in the name: {}\n", value);
			continue;
		}

		mStorageID[convertedStr] = uniformIndex;
		glUniformBlockBinding(key, uniformIndex, pIndex);
	}

	glGenBuffers(1, &mUBOid);
	glBindBuffer(GL_UNIFORM_BUFFER, mUBOid);
	glBufferData(GL_UNIFORM_BUFFER, pSize, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	glBindBufferRange(GL_UNIFORM_BUFFER, pIndex, mUBOid, 0, pSize);
}

void UBO::correlateData(const std::vector<std::pair<uint32_t, std::string_view>>& pUniform,
						uint32_t pIndex)
{
	for (auto& [key, value] : pUniform)
	{
		std::string convertedStr = std::string(value);

		uint32_t uniformIndex = glGetUniformBlockIndex(key, convertedStr.c_str());

		if (uniformIndex == GL_INVALID_INDEX)
		{
			std::cout << std::format("ERROR! The index is incorrect. Maybe an issue in the name: {}\n", value);
			continue;
		}

		mStorageID[convertedStr] = uniformIndex;
		glUniformBlockBinding(key, uniformIndex, pIndex);
	}
}

void UBO::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, mUBOid);
}

void UBO::unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

uint32_t UBO::getID() const noexcept
{
	return mUBOid;
}

uint32_t UBO::getUniformID(std::string_view pName) 
{
	std::string convertedString = std::string(pName);
	
	auto it = mStorageID.find(convertedString);
	if (it == mStorageID.end())
	{
		std::cout << std::format("The shader doesnt contain this uniform: {}!\n", pName);
		return GL_INVALID_INDEX;
	}
	return it->second;
}
