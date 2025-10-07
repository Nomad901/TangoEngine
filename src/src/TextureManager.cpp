#include "TextureManager.h"

TextureManager::TextureManager()
{
	mStorageTextures.reserve(50);
}

TextureManager::TextureManager(std::string_view pName, std::unique_ptr<Texture2> pTexture)
{
	pushTexture(pName, std::move(pTexture));
}

Texture2& TextureManager::operator[](std::string_view pName)
{
	std::string name{ pName };
	assert(mStorageTextures.contains(name));
	return *mStorageTextures[name];
}

const Texture2& TextureManager::operator[](std::string_view pName) const
{
	std::string name{ pName };
	assert(mStorageTextures.contains(name));
	return *mStorageTextures.at(name);
}

void TextureManager::pushTexture(std::string_view pName, std::unique_ptr<Texture2> pTexture)
{
	mStorageTextures[std::string(pName)] = std::move(pTexture);
}

void TextureManager::popTextures(std::string_view pName)
{
	if (mStorageTextures.erase(std::string(pName)) == 0)
		std::cout << std::format("Texture {} doesnt exist!\n", std::string(pName));
}

Texture2& TextureManager::getTexture(std::string_view pName)
{
	std::string name{ pName };
	assert(mStorageTextures.contains(name));
	return *mStorageTextures[name];
}

auto TextureManager::getStorageTextures() noexcept -> std::unordered_map<std::string, std::unique_ptr<Texture2>>&
{
	return mStorageTextures;
}

