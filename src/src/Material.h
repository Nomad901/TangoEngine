#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Texture2.h"

class Texture2;

class Material
{
public:
	Material() = default;
	Material(glm::vec3 pAmbient,
			 glm::vec3 pDiffuse,
			 glm::vec3 pSpecular,
			 float pShines);

	void init(glm::vec3 pAmbient,
			  glm::vec3 pDiffuse,
			  glm::vec3 pSpecular,
			  float pShines);

	void sendToShader(Shader& pShader, std::pair<uint32_t, uint32_t> pSlots,
					  uint32_t pDiffuseIndex, uint32_t pSpecularIndex);
	void sendToShaderColored(Shader& pShader);

	void setAmbient(const glm::vec3& pAmbient);
	void setDiffuse(const glm::vec3& pDiffuse);
	void setSpecular(const glm::vec3& pSpecular);
	void setShines(float pShines);

	glm::vec3 getAmbient() const noexcept;
	glm::vec3 getDiffuse() const noexcept;
	glm::vec3 getSpecular() const noexcept;
	float getShines() const noexcept;

private:
	float mShines{ 32 };
	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;
};

