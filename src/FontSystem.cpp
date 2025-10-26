#include "FontSystem.h"

FontSystem::FontSystem(const std::filesystem::path& pFontPath)
{
	init(pFontPath);
}

FontSystem::~FontSystem()
{
	for (auto& [key, value] : mCharacters)
	{
		glDeleteTextures(1, &value.mTextureID);
	}
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

void FontSystem::init(const std::filesystem::path& pFontPath)
{
	// ft part
	FT_Library ftLib;
	if (FT_Init_FreeType(&ftLib))
	{
		std::cout << "Couldnt init FT_library!\n";
		return;
	}
	if (!std::filesystem::exists(pFontPath))
	{
		std::cout << std::format("Font {} doesnt exist!\n", pFontPath.string());
		return;
	}
	FT_Face ftFace;
	if (FT_New_Face(ftLib, pFontPath.string().c_str(), 0, &ftFace))
	{
		std::cout << std::format("Couldnt init ftFace! The FontPath: {}\n", pFontPath.string());
		return;
	}

	initCharacters(ftFace);

	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLib);

	// other parts
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initShader();
	initQuad();
}

void FontSystem::renderText(uint32_t pWinWidth, uint32_t pWinHeight, 
							std::string_view pText, glm::vec2 pPos, float pScale, const glm::vec3& pColor)
{
	if (mIsShown)
	{
		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(pWinWidth), 0.0f, static_cast<float>(pWinHeight));
		mShader.bind();
		mShader.setMatrixUniform4fv("uProj", proj);
		mShader.setUniform3fv("uTextColor", pColor);
		glBindVertexArray(mVAO);
		glActiveTexture(GL_TEXTURE0);

		std::string text = std::string(pText);
		for (auto& glyphIt : text)
		{
			Character character = mCharacters[glyphIt];

			float xPos = pPos.x + character.mBearing.x * pScale;
			float yPos = pPos.y - (character.mSize.y - character.mBearing.y) * pScale;

			float width = character.mSize.x * pScale;
			float height = character.mSize.y * pScale;

			float vertices[6][4] =
			{
				{xPos,		   yPos + height, 0.0f, 0.0f},
				{xPos,		   yPos,		  0.0f, 1.0f},
				{xPos + width, yPos,		  1.0f, 1.0f},

				{xPos,		   yPos + height, 0.0f, 0.0f},
				{xPos + width, yPos,		  1.0f, 1.0f},
				{xPos + width, yPos + height, 1.0f, 0.0f}
			};

			glBindTexture(GL_TEXTURE_2D, character.mTextureID);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			pPos.x += (character.mAdvance >> 6) * pScale;
		}
	}
}

void FontSystem::makeShown(bool pShown)
{
	mIsShown = pShown;
}

bool FontSystem::isShown() const noexcept
{
	return mIsShown;
}

void FontSystem::initShader()
{
	std::string resourcesPath = RESOURCES_PATH;
	mShader.init(resourcesPath + "Shaders/glyphShaderVert.glsl",
				 resourcesPath + "Shaders/glyphShaderFrag.glsl");
}

void FontSystem::initQuad()
{
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FontSystem::initCharacters(FT_Face& pFT_Face)
{
	// setting width on zero - dynamically will define the width according to the height;
	FT_Set_Pixel_Sizes(pFT_Face, 0, 48);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (uint8_t glyph = 0; glyph < 128; ++glyph)
	{
		if (FT_Load_Char(pFT_Face, glyph, FT_LOAD_RENDER))
		{
			std::cout << std::format("Failed to load the glyph: {}\n", glyph);
			return;
		}
		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 
					 pFT_Face->glyph->bitmap.width, 
					 pFT_Face->glyph->bitmap.rows, 
					 0, GL_RED, GL_UNSIGNED_BYTE, 
					 pFT_Face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Character character =
		{
			texture,
			static_cast<uint32_t>(pFT_Face->glyph->advance.x),
			glm::ivec2(pFT_Face->glyph->bitmap.width, pFT_Face->glyph->bitmap.rows),
			glm::ivec2(pFT_Face->glyph->bitmap_left, pFT_Face->glyph->bitmap_top)
		};
		mCharacters.insert_or_assign(glyph, character);
	}
}

