#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "VBO.h"
#include "VAO.h"
#include "VBOLayout.h"
#include "EBO.h"
#include "Texture2.h"
#include "Shader.h"

class Primitive
{
public:
	Primitive() = default;
	virtual ~Primitive() = default;

	void setVertexStrg(const std::vector<Vertex>& pVertexStrg);
	void setIndexStrg(const std::vector<uint32_t>& pIndexStrg); 
	void setTexture(const Texture2& pTexture);
	void setTexSloth(uint32_t pTexSloth);

	uint32_t getTexSloth() noexcept;
	std::vector<Vertex>& getVertexStrg() noexcept;
	std::vector<uint32_t>& getIndexStrg() noexcept;
	Texture2& getTexture() noexcept;

private:
	uint32_t mTexSloth{};
	std::vector<Vertex> mVertexStrg;
	std::vector<uint32_t> mIndexStrg;
	Texture2 mTexture;

};

class Triangle : public Primitive
{
public:
	Triangle(const Texture2& pTexture, uint32_t pSloth);
	Triangle(const Texture2& pTexture, uint32_t pSloth, const glm::vec4& pColor);
	Triangle(const glm::vec4& pColor);

};

class Pyramid : public Primitive
{
public:
	Pyramid(const Texture2& pTexture, uint32_t pSloth);
	Pyramid(const Texture2& pTexture, uint32_t pSloth, const glm::vec4& pColor);
	Pyramid(const glm::vec4& pColor);

};

class Quad : public Primitive
{
public:
	Quad(const Texture2& pTexture, uint32_t pSloth);
	Quad(const Texture2& pTexture, uint32_t pSloth, const glm::vec4& pColor);
	Quad(const glm::vec4& pColor);
};

class Cube : public Primitive
{
public:
	Cube(const Texture2& pTexture, uint32_t pSloth);
	Cube(const Texture2& pTexture, uint32_t pSloth, const glm::vec4& pColor);
	Cube(const glm::vec4& pColor);

};