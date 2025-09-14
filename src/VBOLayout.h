#pragma once
#include <iostream>
#include <vector>

#include "glad/glad.h"

struct VBOLayoutComponents
{
	uint8_t mNormalized;
	uint32_t mType;
	uint32_t mCount;

	static uint32_t sizeOf(uint32_t pType)
	{
		switch (pType)
		{
		case GL_FLOAT:
			return sizeof(GL_FLOAT);
		case GL_UNSIGNED_BYTE:
			return sizeof(GL_UNSIGNED_BYTE);
		case GL_BYTE:
			return sizeof(GL_BYTE);
		case GL_UNSIGNED_INT:
			return sizeof(GL_UNSIGNED_INT);
		}
		return sizeof(GL_FLOAT);
	}
};

class VBOLayout
{
public:
	VBOLayout() = default;

	void pushLayout(uint32_t pType, uint32_t pCount);

	auto getComponentsOfLayout() -> std::vector<VBOLayoutComponents>&;
	uint32_t getCount() const noexcept;
private:
	uint32_t mCount{};
	std::vector<VBOLayoutComponents> mVBOLayoutComponents;

};

