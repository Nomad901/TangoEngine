#include "PBRMaterial.h"

uint32_t PBRMaterial::getIndex(TEXTURE_TYPE pTEXTURE_TYPE)
{
    return static_cast<uint32_t>(pTEXTURE_TYPE);
}
