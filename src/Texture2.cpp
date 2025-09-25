#include "Texture2.h"

Texture2::Texture2(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture)
{
	init(pPath, pUniformName, pRepeatTexture);
}

Texture2::~Texture2()
{
	std::cout << std::format("Texture was deleted! ID: {}\n", mRendererID);
	glDeleteTextures(1, &mRendererID);
}

Texture2::Texture2(Texture2&& pAnotherTexture) noexcept
{
	mWidth = pAnotherTexture.mWidth;
	mHeight = pAnotherTexture.mHeight;
	mBPP = pAnotherTexture.mBPP;
	mRendererID = pAnotherTexture.mRendererID;
	mLocalBuffer = pAnotherTexture.mLocalBuffer;
	mType = pAnotherTexture.mType;
	mUniformName = std::move(pAnotherTexture.mUniformName);
	mFilePath = std::move(pAnotherTexture.mFilePath);

	pAnotherTexture.mWidth = 0;
	pAnotherTexture.mHeight = 0;
	pAnotherTexture.mBPP = 0;
	pAnotherTexture.mRendererID = 0;
	pAnotherTexture.mLocalBuffer = nullptr;
}

Texture2& Texture2::operator=(Texture2&& pAnotherTexture) noexcept
{
	if (&pAnotherTexture == this)
		return *this;

	if (mRendererID != 0)
		glDeleteTextures(1, &mRendererID);
	delete mLocalBuffer;

	mWidth = pAnotherTexture.mWidth;
	mHeight = pAnotherTexture.mHeight;
	mBPP = pAnotherTexture.mBPP;
	mRendererID = pAnotherTexture.mRendererID;
	mLocalBuffer = pAnotherTexture.mLocalBuffer;
	mType = pAnotherTexture.mType;
	mUniformName = std::move(pAnotherTexture.mUniformName);
	mFilePath = std::move(pAnotherTexture.mFilePath);
	
	pAnotherTexture.mWidth = 0;
	pAnotherTexture.mHeight = 0;
	pAnotherTexture.mBPP = 0;
	pAnotherTexture.mRendererID = 0;
	pAnotherTexture.mLocalBuffer = nullptr;

	return *this;
}

void Texture2::init(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture)
{
	mUniformName = pUniformName;
	mFilePath = pPath;
	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(pPath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);

	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_2D, mRendererID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (pRepeatTexture)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

void Texture2::init(const std::filesystem::path& pPath, bool pRepeatTexture)
{
	mFilePath = pPath;
	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(pPath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);

	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_2D, mRendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (pRepeatTexture)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

void Texture2::initWithMSAA(const std::filesystem::path& pPath, std::string_view pUniformName, uint32_t pSamples)
{
	mUniformName = pUniformName;
	mFilePath = pPath;
	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(pPath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);

	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mRendererID);
	
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, pSamples, GL_RGB, mWidth, mHeight, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

void Texture2::initWithMSAA(const std::filesystem::path& pPath, uint32_t pSamples)
{
	mFilePath = pPath;
	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(pPath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);

	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mRendererID);

	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, pSamples, GL_RGB, mWidth, mHeight, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

void Texture2::initEmpty(int32_t pWidth, int32_t pHeight)
{
	mWidth = pWidth;
	mHeight = pHeight;

	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_2D, mRendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2::initCubeMaps(const std::array<std::filesystem::path, 6>& pPaths)
{
	stbi_set_flip_vertically_on_load(false);

	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererID);
	
	int32_t width, height, nrChannels;
	for (size_t i = 0; i < pPaths.size(); ++i)
	{
		mLocalBuffer = stbi_load(pPaths[i].string().c_str(), &width, &height, &nrChannels, 0);
		if (mLocalBuffer)
		{			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
						 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, mLocalBuffer);
			stbi_image_free(mLocalBuffer);
		}
		else
		{
			std::cout << "couldnt load the image\n";
			break;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture2::bind(GLenum pTarget, uint32_t pSlot)
{
	glActiveTexture(GL_TEXTURE0 + pSlot);
	glBindTexture(pTarget, mRendererID);
}

void Texture2::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::string Texture2::getUniformName() const noexcept
{
	return mUniformName;
}

void Texture2::setID(uint32_t pID)
{
	mRendererID = pID;
}

void Texture2::setType(std::string_view pType)
{
	mType = pType;
}

void Texture2::setPath(const std::filesystem::path& pPath)
{
	mFilePath = pPath;
}

uint32_t Texture2::getID() const noexcept
{
	return mRendererID;
}

std::string Texture2::getType() const noexcept
{
	return mType;
}

std::filesystem::path Texture2::getPath() const noexcept
{
	return mFilePath;
}

void Texture2::destroyTexture()
{
	glDeleteTextures(1, &mRendererID);
}
