#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

class Texture2;

class Material
{
public:
	Material() = default;
	Material(glm::vec3 pAmbient,
			 glm::vec3 pDiffuse,
			 glm::vec3 pSpecular,
			 float pShines,
			 std::vector<Texture2>& pTextures);

	void init(glm::vec3 pAmbient,
			  glm::vec3 pDiffuse,
			  glm::vec3 pSpecular,
			  float pShines,
			  std::vector<Texture2>& pTextures);

	void sendToShader(Shader& pShader, bool isJustColored = false);

	void setAmbient(const glm::vec3& pAmbient);
	void setDiffuse(const glm::vec3& pDiffuse);
	void setSpecular(const glm::vec3& pSpecular);
	void setShines(float pShines);
	void setCurrentIndex(int32_t pIndex);

	glm::vec3 getAmbient() const noexcept;
	glm::vec3 getDiffuse() const noexcept;
	glm::vec3 getSpecular() const noexcept;
	float getShines() const noexcept;
	int32_t getCurrentIndex() const noexcept;

private:
	float mShines{ 32 };
	int32_t mCurrentIndex{ 0 };
	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;
	std::vector<Texture2> mTextures;

};

