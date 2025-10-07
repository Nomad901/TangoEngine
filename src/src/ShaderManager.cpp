#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	mStorageShaders.reserve(50);
}

ShaderManager::ShaderManager(std::string_view pName, const std::filesystem::path& pPathVertex,
							 const std::filesystem::path& pPathFragment, 
							 const std::filesystem::path& pGeometry)
{
	pushShader(pName, std::make_unique<Shader>(pPathVertex, pPathFragment, pGeometry));
}

ShaderManager::ShaderManager(std::string_view pName, std::unique_ptr<Shader> pShader)
{
	pushShader(pName, std::move(pShader));
}

Shader& ShaderManager::operator[](std::string_view pName)
{
	std::string name{ pName };
	assert(mStorageShaders.contains(name));
	return *mStorageShaders[name];
}

const Shader& ShaderManager::operator[](std::string_view pName) const
{
	std::string name{ pName };
	assert(mStorageShaders.contains(name));
	return *mStorageShaders.at(name);
}

void ShaderManager::pushShader(std::string_view pName, const std::filesystem::path& pPathVertex, 
							   const std::filesystem::path& pPathFragment, 
							   const std::filesystem::path& pGeometry)
{
	std::string name{ pName };
	if (pGeometry != std::filesystem::current_path())
		mStorageShaders[name] = std::make_unique<Shader>(pPathVertex, pPathFragment, pGeometry);
	else 
		mStorageShaders[name] = std::make_unique<Shader>(pPathVertex, pPathFragment);
}

void ShaderManager::pushShader(std::string_view pName, std::unique_ptr<Shader> pShader)
{
	mStorageShaders[std::string(pName)] = std::move(pShader);
}

void ShaderManager::popShader(std::string_view pName)
{
	if (mStorageShaders.erase(std::string(pName)) == 0)
		std::cout << std::format("Shader with this name: {} wasnt found!\n", pName);
}

Shader& ShaderManager::getShader(std::string_view pName)
{
	std::string name{ pName };
	assert(mStorageShaders.contains(name));
	return *mStorageShaders[name];
}

auto ShaderManager::getStorageShaders() noexcept -> std::unordered_map<std::string, std::unique_ptr<Shader>>&
{
	return mStorageShaders;
}
