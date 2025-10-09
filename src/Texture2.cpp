#include "Texture2.h"

Texture2::Texture2(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture)
{
	init(pPath, pUniformName, pRepeatTexture);
}

Texture2::Texture2(const std::filesystem::path& pPath, bool pRepeatTexture)
{
	init(pPath, pRepeatTexture);
}

Texture2::Texture2(GLenum pTarget)
{
	init(pTarget);
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

void Texture2::init(GLenum pTarget)
{
	mTarget = pTarget;
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

void Texture2::loadRaw(int32_t pWidth, int32_t pHeight, int32_t pBPP, uint8_t* pImageData, bool pIsRGB)
{
	mWidth = pWidth;
	mHeight = pHeight;
	mBPP = pBPP;
	
	loadInternal(pImageData, pIsRGB);
}

void Texture2::bind(uint32_t pSlot)
{
	glActiveTexture(GL_TEXTURE0 + pSlot);
	glBindTexture(mTarget, mRendererID);
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

void Texture2::setLocalBuffer(uint8_t* pLocalBuffer)
{
	mLocalBuffer = pLocalBuffer;
}

void Texture2::setWidth(int32_t pWidth)
{
	mWidth = pWidth;
}

void Texture2::setHeight(int32_t pHeight)
{
	mHeight = pHeight;
}

void Texture2::setBPP(int32_t pBPP)
{
	mBPP = pBPP;
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

int32_t Texture2::getWidth() const noexcept
{
	return mWidth;
}

int32_t Texture2::getHeight() const noexcept
{
	return mHeight;
}

int32_t Texture2::getBPP() const noexcept
{
	return mBPP;
}

uint8_t* Texture2::getLocalBuffer() noexcept
{
	return mLocalBuffer;
}

uint64_t Texture2::getBindlessHandle() const noexcept
{
	return mBindlessHandle;
}

void Texture2::destroyTexture()
{
	glDeleteTextures(1, &mRendererID);
}

void Texture2::loadInternal(const void* pImageData, bool pIsRGB)
{
	int32_t majorVersion, minorVersion;
	Utils::getInstance().getGLVersion(majorVersion, minorVersion);
	if (majorVersion == 4 && minorVersion > 5)
		loadInternalDSA(pImageData, pIsRGB);
	else
		loadNonInternalDSA(pImageData, pIsRGB);
}

void Texture2::loadInternalDSA(const void* pImageData, bool pIsRGB)
{
	glCreateTextures(mTarget, 1, &mRendererID);
	int32_t levels = std::min(5, static_cast<int32_t>(log2f(static_cast<float>(std::max(mWidth, mHeight)))));
	GLenum internalFormat = GL_NONE;

	if (mTarget == GL_TEXTURE_2D)
	{
		// TODO: if for ktx textures;
		switch (mBPP)
		{
		case 1: 
		{
			glTextureStorage2D(mRendererID, levels, GL_R8, mWidth, mHeight);
			glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, GL_RED, GL_UNSIGNED_BYTE, pImageData);
			GLint swizzleMask[] = { GL_RED, GL_RED ,GL_RED ,GL_RED };
			glTextureParameteriv(mRendererID, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		} 
		break;
		case 2:
			glTextureStorage2D(mRendererID, levels, GL_RG8, mWidth, mHeight);
			glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, GL_RG, GL_UNSIGNED_BYTE, pImageData);
			break;
		case 3:
			internalFormat = pIsRGB ? GL_SRGB8 : GL_RGB8;
			glTextureStorage2D(mRendererID, levels, internalFormat, mWidth, mHeight);
			glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
			break;
		case 4:
			internalFormat = pIsRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
			glTextureStorage2D(mRendererID, levels, internalFormat, mWidth, mHeight);
			glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
			break;
		default:
			std::cout << "other levels of BPP are not implemented yet :(\n";
			break;
		}
	}
	else
	{
		std::cout << "The texture doesnt support other targets than GL_TEXTURE_2D!\n";
		return;
	}

	glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(mRendererID, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(mRendererID, GL_TEXTURE_MAX_LEVEL, levels - 1);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, 16);
	
	glGenerateTextureMipmap(mRendererID);
	
	mBindlessHandle = glGetTextureHandleARB(mRendererID);
	glMakeTextureHandleNonResidentARB(mBindlessHandle);
}

void Texture2::loadNonInternalDSA(const void* pImageData, bool pIsRGB)
{
	glGenTextures(1, &mRendererID);
	glBindTexture(mTarget, mRendererID);
	
	GLenum internalFormat = GL_NONE;

	if (mTarget == GL_TEXTURE_2D)
	{
		switch (mBPP)
		{
		case 1:
		{
			glTexImage2D(mRendererID, 0, GL_RED, mWidth, mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, pImageData);
			GLint swizzleMask[] = { GL_RED,GL_RED ,GL_RED ,GL_RED };
			glTexParameteriv(mTarget, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		break;
		case 2:
			glTexImage2D(mRendererID, 0, GL_RG, mWidth, mHeight, 0, GL_RG, GL_UNSIGNED_BYTE, pImageData);
			break;
		case 3:
			internalFormat = pIsRGB ? GL_SRGB8 : GL_RGB8;
			glTexImage2D(mRendererID, 0, GL_RGB, mWidth, mHeight, 0, internalFormat, GL_UNSIGNED_BYTE, pImageData);
			break;
		case 4:
			internalFormat = pIsRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
			glTexImage2D(mRendererID, 0, GL_RGBA, mWidth, mHeight, 0, internalFormat, GL_UNSIGNED_BYTE, pImageData);
			break;
		default:
			std::cout << "Other chanels of bpp are not implented!\n";
			break;
		}
	}
	else
	{
		std::cout << "The texture doesnt support other targets than GL_TEXTURE_2D!\n";
		return;
	}
	glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(mRendererID, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(mTarget);

	glBindTexture(mTarget, 0);
}

terrainTexture::terrainTexture(const std::filesystem::path& pPath)
{
	init(pPath);
}

void terrainTexture::init(const std::filesystem::path& pPath)
{
	Texture2::setPath(pPath);
	stbi_set_flip_vertically_on_load(1);
	int32_t width, height, bpp;
	uint8_t* localBuffer = stbi_load(pPath.string().c_str(), &width, &height, &bpp, 0);
	Texture2::setWidth(width);
	Texture2::setHeight(height);
	Texture2::setBPP(bpp);
	Texture2::setLocalBuffer(localBuffer);

	mVertices.reserve(height * width * sizeof(glm::vec3));
	float yScale = 64.0f / 256.0f, yShift = 16.0f;
	for (uint32_t i = 0; i < height; ++i)
	{
		for (uint32_t j = 0; j < width; ++j)
		{
			uint8_t* texel = localBuffer + (j + width * i) * bpp;
			uint8_t y = texel[0];

			mVertices.push_back(glm::vec3(
				j - width / 2.0f,                  
				(int32_t)y * yScale - yShift,      
				i - height / 2.0f                  
			));
		}
	}
	if (Texture2::getLocalBuffer())
		stbi_image_free(Texture2::getLocalBuffer());

	mIndices.reserve((height - 1) * (width * 2 - 2));
	for (uint32_t i = 0; i < height - 1; ++i)
	{
		for (uint32_t j = 0; j < width; ++j)
		{
			for (uint32_t k = 0; k < 2; ++k)
			{
				mIndices.push_back(j + width * (i + k));
			}
		}
	}
}

std::vector<glm::vec3>& terrainTexture::getVertices() noexcept
{
	return mVertices;
}

std::vector<uint64_t>& terrainTexture::getIndices() noexcept
{
	return mIndices;
}
