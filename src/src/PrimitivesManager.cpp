#include "PrimitivesManager.h"

PrimitivesManager::PrimitivesManager()
{
	mStoragePrimitives.reserve(50);
}

PrimitivesManager::PrimitivesManager(std::string_view pName, std::shared_ptr<Primitive> pPrimitive)
{
	pushPrimitive(pName, std::move(pPrimitive));
}

std::shared_ptr<Primitive>& PrimitivesManager::operator[](std::string_view pName)
{
	std::string name{ pName };
	assert(mStoragePrimitives.contains(name));
	return mStoragePrimitives[name];
}

const std::shared_ptr<Primitive>& PrimitivesManager::operator[](std::string_view pName) const
{
	std::string name{ pName };
	assert(mStoragePrimitives.contains(name));
	return mStoragePrimitives.at(name);
}

void PrimitivesManager::pushPrimitive(std::string_view pName, std::shared_ptr<Primitive> pPrimitive)
{
	mStoragePrimitives[std::string(pName)] = std::move(pPrimitive);
}

void PrimitivesManager::popPrimitive(std::string_view pName)
{
	if (mStoragePrimitives.erase(std::string(pName)) == 0)
		std::cout << std::format("Primitive {} doesnt exist!\n", pName);
}

std::shared_ptr<Primitive>& PrimitivesManager::getPrimitive(std::string_view pName)
{
	std::string name{ pName };
	assert(mStoragePrimitives.contains(name));
	return mStoragePrimitives[name];
}

auto PrimitivesManager::getStoragePrimitives() noexcept -> std::unordered_map<std::string, std::shared_ptr<Primitive>>&
{
	return mStoragePrimitives;
}
