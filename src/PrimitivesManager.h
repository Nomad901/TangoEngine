#pragma once
#include <iostream>
#include <format>
#include <unordered_map>
#include <string>

#include "Primitive.h"

class PrimitivesManager
{
public:
	PrimitivesManager();
	PrimitivesManager(std::string_view pName, std::shared_ptr<Primitive> pPrimitive);
	~PrimitivesManager() = default;
	PrimitivesManager(const PrimitivesManager&) = delete;
	PrimitivesManager& operator=(const PrimitivesManager&) = delete;
	PrimitivesManager(PrimitivesManager&&) noexcept = default;
	PrimitivesManager& operator=(PrimitivesManager&&) = default;
	std::shared_ptr<Primitive>& operator[](std::string_view pName);
	const std::shared_ptr<Primitive>& operator[](std::string_view pName) const;

	void pushPrimitive(std::string_view pName, std::shared_ptr<Primitive> pPrimitive);
	void popPrimitive(std::string_view pName);
	std::shared_ptr<Primitive>& getPrimitive(std::string_view pName);

	auto getStoragePrimitives() noexcept -> std::unordered_map<std::string, std::shared_ptr<Primitive>>&;

private:
	std::unordered_map<std::string, std::shared_ptr<Primitive>> mStoragePrimitives;
};

