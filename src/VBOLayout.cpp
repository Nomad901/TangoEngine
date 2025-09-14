#include "VBOLayout.h"

void VBOLayout::pushLayout(uint32_t pType, uint32_t pCount)
{
    mVBOLayoutComponents.push_back({ GL_FALSE, pType, pCount });
    mCount += pCount * VBOLayoutComponents::sizeOf(pType);
}

auto VBOLayout::getComponentsOfLayout() -> std::vector<VBOLayoutComponents>&
{
    return mVBOLayoutComponents;
}

uint32_t VBOLayout::getCount() const noexcept
{
    return mCount;
}
