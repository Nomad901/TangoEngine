#pragma once
#include <iostream>
#include <format>
#include <filesystem>

#include "glm/glm.hpp"
#include "dependencies/stb_image.h"
#include "dependencies/stb_image_write.h"

class STBImage
{
public:
	STBImage() = default;
	STBImage(const std::filesystem::path& pPath);
	~STBImage();

	void load(const std::filesystem::path& pPath);
	void unload();

	glm::vec3 getColor(int32_t pX, int32_t pY) const noexcept;

private:
	int32_t mWidth{}, mHeight{}, mBPP{};
	uint8_t* mBufferData;
};

