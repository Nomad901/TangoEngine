#include "Material.h"

uint32_t Material::getIndex(TEXTURE_TYPE pTextureType)
{
	return static_cast<uint32_t>(pTextureType);
}
