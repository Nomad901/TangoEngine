#include "ModelManager.h"

ModelManager::ModelManager()
{
	mStorageModels.reserve(50);
}

ModelManager::ModelManager(std::string_view pName, std::unique_ptr<Model> pModel)
{
	pushModel(pName, std::move(pModel));
}

Model& ModelManager::operator[](std::string_view pName)
{
	std::string name{ pName };
	assert(mStorageModels.contains(name));
	return *mStorageModels[name];
}

const Model& ModelManager::operator[](std::string_view pName) const
{
	std::string name{ pName };
	assert(mStorageModels.contains(name));
	return *mStorageModels.at(name);
}

void ModelManager::pushModel(std::string_view pName, std::unique_ptr<Model> pModel)
{
	mStorageModels[std::string(pName)] = std::move(pModel);
}

void ModelManager::popModel(std::string_view pName)
{
	if (mStorageModels.erase(std::string(pName)) == 0)
		std::cout << std::format("Model {} doesnt exist!\n", pName);
}

Model& ModelManager::getModel(std::string_view pName)
{
	std::string name{ pName };
	assert(mStorageModels.contains(name));
	return *mStorageModels[name];
}

auto ModelManager::getStorageModels() noexcept -> std::unordered_map<std::string, std::unique_ptr<Model>>&
{
	return mStorageModels;
}
