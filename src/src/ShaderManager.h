#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <unordered_map>
#include <string>

#include "Shader.h"

class ShaderManager
{
public:
	ShaderManager();
	ShaderManager(std::string_view pName, const std::filesystem::path& pPathVertex, 
				  const std::filesystem::path& pPathFragment,
				  const std::filesystem::path& pGeometry = std::filesystem::current_path());
	ShaderManager(std::string_view pName, std::unique_ptr<Shader> pShader);
	~ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) noexcept = default;
	ShaderManager& operator=(ShaderManager&&) = default;
	Shader& operator[](std::string_view pName);
	const Shader& operator[](std::string_view pName) const;

	void pushShader(std::string_view pName, const std::filesystem::path& pPathVertex, 
					const std::filesystem::path& pPathFragment, 
				    const std::filesystem::path& pGeometry = std::filesystem::current_path());
	void pushShader(std::string_view pName, std::unique_ptr<Shader> pShader);
	void popShader(std::string_view pName);
	Shader& getShader(std::string_view pName);
	
	auto getStorageShaders() noexcept -> std::unordered_map<std::string, std::unique_ptr<Shader>>&;

private:
	std::unordered_map<std::string, std::unique_ptr<Shader>> mStorageShaders;
	
};
