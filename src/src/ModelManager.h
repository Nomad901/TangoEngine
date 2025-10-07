#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <unordered_map>
#include <string>

#include "Model.h"

class ModelManager
{
public:
	ModelManager();
	ModelManager(std::string_view pName, std::unique_ptr<Model> pModel);
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
	ModelManager(ModelManager&&) noexcept = default;
	ModelManager& operator=(ModelManager&&) = default;
	Model& operator[](std::string_view pName);
	const Model& operator[](std::string_view pName) const;

	void pushModel(std::string_view pName, std::unique_ptr<Model> pModel);
	void popModel(std::string_view pName);
	Model& getModel(std::string_view pName);

	auto getStorageModels() noexcept -> std::unordered_map<std::string, std::unique_ptr<Model>>&;

private:
	std::unordered_map<std::string, std::unique_ptr<Model>> mStorageModels;

};

