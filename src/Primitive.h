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

	void setTexture(const std::pair<Texture2, Texture2>& pTexture);
	void setTexSlots(std::pair<uint32_t, uint32_t> pTexSlots);

	std::vector<Vertex>& getVertexStrg() noexcept;
	std::vector<uint32_t>& getIndexStrg() noexcept;
	std::pair<uint32_t, uint32_t> getTexSlots() const noexcept;
	std::pair<Texture2, Texture2>& getTextures() noexcept;

private:
	std::vector<Vertex> mVertexStrg;
	std::vector<uint32_t> mIndexStrg;

	std::pair<uint32_t, uint32_t> mTexSlots{};
	std::pair<Texture2, Texture2> mTextures;

};

class Triangle : public Primitive
{
public:
	Triangle(const std::pair<Texture2, Texture2>& pTexture, 
				   std::pair<uint32_t, uint32_t> pSlots);
	Triangle(const std::pair<Texture2, Texture2>& pTexture, 
				   std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Triangle(const glm::vec4& pColor);

};

class Pyramid : public Primitive
{
public:
	Pyramid(const std::pair<Texture2, Texture2>& pTexture, 
				  std::pair<uint32_t, uint32_t> pSlots);
	Pyramid(const std::pair<Texture2, Texture2>& pTexture, 
				  std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Pyramid(const glm::vec4& pColor);

};

class Quad : public Primitive
{
public:
	Quad(const std::pair<Texture2, Texture2>& pTexture, 
			   std::pair<uint32_t, uint32_t> pSlots);
	Quad(const std::pair<Texture2, Texture2>& pTexture, 
			   std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Quad(const glm::vec4& pColor);
};

class Cube : public Primitive
{
public:
	Cube(const std::pair<Texture2, Texture2>& pTexture, 
			   std::pair<uint32_t, uint32_t> pSlots, bool pForSkybox);
	Cube(const std::pair<Texture2, Texture2>& pTexture, 
			   std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Cube(const glm::vec4& pColor);

};

class Sphere : public Primitive
{
public:
	Sphere(const std::pair<Texture2, Texture2>& pTexture, 
				 std::pair<uint32_t, uint32_t> pSlots);
	Sphere(const std::pair<Texture2, Texture2>& pTexture, 
				 std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Sphere(const glm::vec4& pColor);

};