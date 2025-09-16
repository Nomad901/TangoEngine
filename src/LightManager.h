#pragma once
#include <iostream>
#include <format>
#include <unordered_map>
#include <string>
#include <memory>
#include <expected>

#include "Light.h"

class LightManager
{
public:
	LightManager() = default;
	LightManager(std::string_view pNameLight, std::unique_ptr<Light>&& pLight);
	
	auto pushLight(std::string_view pNameLight, std::unique_ptr<Light>&& pLight) -> void;
	auto popLight(std::string_view pNameLight) -> void;
	auto getLight(std::string_view pNameLight) -> std::expected<std::unique_ptr<Light>&, std::string_view>;
	auto getStorageLight() const noexcept -> const std::unordered_map<std::string, std::unique_ptr<Light>>;
	auto getSize() const noexcept -> size_t;
	
	auto sendAllToShader(Shader& pShader);

private:
	std::unordered_map<std::string, std::unique_ptr<Light>> mStrgLight;

};

