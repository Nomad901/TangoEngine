#pragma once
#include <iostream>
#include <format>
#include <unordered_map>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Shader.h"

class FontSystem
{
public:
	FontSystem() = default;
	FontSystem(const std::filesystem::path& pFontPath);
	~FontSystem();

	void init(const std::filesystem::path& pFontPath);

	void renderText(uint32_t pWinWidth, uint32_t pWinHeight, 
					std::string_view pText, glm::vec2 pPos, float pScale, const glm::vec3& pColor);

	void makeShown(bool pShown);
	bool isShown() const noexcept;

private:
	void initShader();
	void initQuad();
	void initCharacters(FT_Face& pFT_Face);

private: 
	struct Character
	{
		uint32_t mTextureID;
		uint32_t mAdvance; // offset to next glyph
		glm::ivec2 mSize;
		glm::ivec2 mBearing; // offset from baseline to top/left glyph
	};
private:
	bool mIsShown{ true };

	Shader mShader;
	std::unordered_map<char, Character> mCharacters;

	uint32_t mVAO;
	uint32_t mVBO;
};

