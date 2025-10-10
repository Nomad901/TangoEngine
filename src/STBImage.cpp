#include "STBImage.h"

STBImage::STBImage(const std::filesystem::path& pPath)
{
	load(pPath);
}

STBImage::~STBImage()
{
	unload();
}

void STBImage::load(const std::filesystem::path& pPath)
{
	if (mBufferData)
		unload();
	
	stbi_set_flip_vertically_on_load(1);
	mBufferData = stbi_load(pPath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);
	if (!mBufferData)
	{
		std::cout << std::format("Buffer data couldnt initialize with this path: {}\n", pPath.string());
		return;
	}
}

void STBImage::unload()
{
	if (!mBufferData)
	{
		std::cout << "Trying to unload free buffer data!\n";
		return;
	}
	stbi_image_free(mBufferData);
	mBufferData = nullptr;
	mWidth = 0;
	mHeight = 0;
	mBPP = 0;
}

glm::vec3 STBImage::getColor(int32_t pX, int32_t pY) const noexcept
{
	if (!mBufferData)
	{
		std::cout << std::format("Buffer data is zero, coordinates X/Y: {}/{}\n", pX, pY);
		return glm::vec3(0.0f);
	}

	assert(mWidth > 0 && mHeight > 0);

	int32_t wrappedX = pX % mWidth;
	int32_t wrappedY = pY % mHeight;

	glm::vec3 color;

	uint8_t* colorData = mBufferData + (wrappedY * mWidth + wrappedX) * mBPP;
	color.r = (float)colorData[0];
	color.g = (float)colorData[1];
	color.b = (float)colorData[2];
	return color;
}
