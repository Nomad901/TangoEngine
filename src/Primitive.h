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

	std::vector<Vertex>& getVertexStrg() noexcept;
	std::vector<uint32_t>& getIndexStrg() noexcept;
	Texture2& getTexture() noexcept;

private:
	std::vector<Vertex> mVertexStrg;
	std::vector<uint32_t> mIndexStrg;
	Texture2 mTexture;

};

class Triangle : public Primitive
{
public:
	Triangle(const Texture2& pTexture);
	Triangle(const Texture2& pTexture, const glm::vec4& pColor);
	Triangle(const glm::vec4& pColor);

};

class Pyramid : public Primitive
{
public:
	Pyramid(const Texture2& pTexture);
	Pyramid(const Texture2& pTexture, const glm::vec4& pColor);
	Pyramid(const glm::vec4& pColor);

};

class Quad : public Primitive
{
public:
	Quad(const Texture2& pTexture);
	Quad(const Texture2& pTexture, const glm::vec4& pColor);
	Quad(const glm::vec4& pColor);

};

class Cube : public Primitive
{
public:
	Cube(const Texture2& pTexture);
	Cube(const Texture2& pTexture, const glm::vec4& pColor);
	Cube(const glm::vec4& pColor);

};