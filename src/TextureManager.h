#pragma once
#include <iostream>
#include <format>
#include <unordered_map>
#include <string>

#include "Texture2.h"

class TextureManager
{
public:
	TextureManager();
	TextureManager(std::string_view pName, std::unique_ptr<Texture2> pTexture);
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(TextureManager&&) noexcept = default;
	TextureManager& operator=(TextureManager&&) = default;
	Texture2& operator[](std::string_view pName);
	const Texture2& operator[](std::string_view pName) const;

	void pushTexture(std::string_view pName, std::unique_ptr<Texture2> pTexture);
	void popTextures(std::string_view pName);
	Texture2& getTexture(std::string_view pName);
	
	auto getStorageTextures() noexcept -> std::unordered_map<std::string, std::unique_ptr<Texture2>>&;

private:
	std::unordered_map<std::string, std::unique_ptr<Texture2>> mStorageTextures;
};

