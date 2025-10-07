#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <array>

#include "glad/glad.h"
#include "dependencies/stb_image.h"

#include "Material.h"

class Texture2
{
public:
	Texture2() = default;
	Texture2(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture = false);
	Texture2(const std::filesystem::path& pPath, bool pRepeatTexture = false);
	~Texture2();
	Texture2(const Texture2&) = delete;
	Texture2& operator=(const Texture2&) = delete;
	Texture2(Texture2&& pAnotherTexture) noexcept;
	Texture2& operator=(Texture2&& pAnotherTexture) noexcept;

	void init(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture = false);
	void init(const std::filesystem::path& pPath, bool pRepeatTexture = false);
	void initWithMSAA(const std::filesystem::path& pPath, std::string_view pUniformName, uint32_t pSamples);
	void initWithMSAA(const std::filesystem::path& pPath, uint32_t pSamples);
	void initEmpty(int32_t pWidth, int32_t pHeight);
	void initCubeMaps(const std::array<std::filesystem::path, 6>& pPaths);

	void bind(GLenum pTarget, uint32_t pSlot = 0);
	void unbind();

	std::string getUniformName() const noexcept;

	void setID(uint32_t pID);
	void setType(std::string_view pType);
	void setPath(const std::filesystem::path& pPath);
	void setLocalBuffer(uint8_t* pLocalBuffer);
	void setWidth(int32_t pWidth);
	void setHeight(int32_t pHeight);
	void setBPP(int32_t pBPP);

	uint32_t getID() const noexcept;
	std::string getType() const noexcept;
	std::filesystem::path getPath() const noexcept;
	int32_t getWidth() const noexcept;
	int32_t getHeight() const noexcept;
	int32_t getBPP() const noexcept;
	uint8_t* getLocalBuffer() noexcept;

	void destroyTexture();

private:
	int32_t mWidth{}, mHeight{}, mBPP{};
	uint32_t mRendererID{};
	uint8_t* mLocalBuffer{};

	std::string mType;

	std::string mUniformName;
	std::filesystem::path mFilePath;
};

class terrainTexture : public Texture2
{
public:
	terrainTexture() = default;
	terrainTexture(const std::filesystem::path& pPath);

	void init(const std::filesystem::path& pPath);

	std::vector<glm::vec3>& getVertices() noexcept;
	std::vector<uint64_t>& getIndices() noexcept;

private:
	std::vector<glm::vec3> mVertices;
	std::vector<uint64_t> mIndices;

};