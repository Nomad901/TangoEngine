#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <array>

#include "glad/glad.h"
#include "dependencies/stb_image.h"

#include "Material.h"
#include "Utils.h"

class Texture2
{
public:
	Texture2() = default;
	Texture2(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture = false);
	Texture2(const std::filesystem::path& pPath, bool pRepeatTexture = false);
	Texture2(GLenum pTarget);
	~Texture2();
	Texture2(const Texture2&) = delete;
	Texture2& operator=(const Texture2&) = delete;
	Texture2(Texture2&& pAnotherTexture) noexcept;
	Texture2& operator=(Texture2&& pAnotherTexture) noexcept;

	void init(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture = false);
	void init(const std::filesystem::path& pPath, bool pRepeatTexture = false);
	void init(GLenum pTarget);
	void initWithMSAA(const std::filesystem::path& pPath, std::string_view pUniformName, uint32_t pSamples);
	void initWithMSAA(const std::filesystem::path& pPath, uint32_t pSamples);
	void initEmpty(int32_t pWidth, int32_t pHeight);
	void initCubeMaps(const std::array<std::filesystem::path, 6>& pPaths);

	void loadRaw(int32_t pWidth, int32_t pHeight, int32_t pBPP, uint8_t* pImageData, bool pIsRGB);

	void bind(uint32_t pSlot = 0);
	void unbind();

	std::string getUniformName() const noexcept;

	void setTarget(GLenum pTarget);
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
	uint64_t getBindlessHandle() const noexcept;

	void destroyTexture();

private:
	void loadInternal(const void* pImageData, bool pIsRGB);
	void loadInternalDSA(const void* pImageData, bool pIsRGB);
	void loadNonInternalDSA(const void* pImageData, bool pIsRGB);

private:
	int32_t mWidth{ 0 }, mHeight{ 0 }, mBPP{ 0 };
	uint32_t mRendererID{};
	int64_t mBindlessHandle{ -1 };
	uint8_t* mLocalBuffer{};

	GLenum mTarget;
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