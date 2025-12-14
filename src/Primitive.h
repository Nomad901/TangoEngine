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
	struct vertexContainerForTangentVertices;
public:
	Primitive() = default;
	virtual ~Primitive() = default;

	void setVertexStrg(const std::vector<Vertex>& pVertexStrg);
	void setVertexWithTangentStrg(const std::vector<VertexWithTangent>& pVertexWithTangentStrg);
	void setIndexStrg(const std::vector<uint32_t>& pIndexStrg); 

	void setTexture(Texture2& pTexture);
	void setTextures(const std::pair<Texture2&, Texture2&>& pTexture);
	void setTexSlots(std::pair<uint32_t, uint32_t> pTexSlots);
	void setTexSlot(uint32_t pSlot);

	std::vector<Vertex>& getVertexStrg() noexcept;
	std::vector<VertexWithTangent>& getVertexWithTangentStrg() noexcept;
	std::vector<uint32_t>& getIndexStrg() noexcept;
	std::pair<uint32_t, uint32_t> getTexSlots() const noexcept;
	uint32_t getSingleSlot() const noexcept;
	Texture2& getFirstTex() noexcept;
	Texture2& getSecondTex() noexcept;
	Texture2& getSingleTex() noexcept;
	
protected:
	const std::vector<VertexWithTangent>& getVerticesWithTangentForQuad(const std::vector<vertexContainerForTangentVertices>& pVertices) noexcept;
	const std::vector<VertexWithTangent>& getVerticesWithTangent(const std::vector<vertexContainerForTangentVertices>& pVertices);

private:
	struct vertexContainerForTangentVertices
	{
		glm::vec3 mPos;
		glm::vec3 mNormal;
		glm::vec2 mUV;
	};

private:
	Texture2 mTexture;
	uint32_t mSlot;
	std::vector<Vertex> mVertexStrg;
	std::vector<VertexWithTangent> mVertexWithTangentStrg;
	std::vector<VertexWithTangent> mCachedResult;
	std::vector<uint32_t> mIndexStrg;

	std::pair<uint32_t, uint32_t> mTexSlots{};
	std::pair<Texture2, Texture2> mTextures;
};

class Triangle : public Primitive
{
public:
	Triangle(const std::pair<Texture2&, Texture2&>& pTexture,
			 std::pair<uint32_t, uint32_t> pSlots);
	Triangle(Texture2& pTexture, uint32_t pSlot, bool pWithTangent = false);
	Triangle(const std::pair<Texture2&, Texture2&>& pTexture,
			 std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Triangle(const glm::vec4& pColor);

};

class Pyramid : public Primitive
{
public:
	Pyramid(const std::pair<Texture2&, Texture2&>& pTexture,
			std::pair<uint32_t, uint32_t> pSlots);
	Pyramid(Texture2& pTexture, uint32_t pSlot, bool pWithTangent = false);
	Pyramid(const std::pair<Texture2&, Texture2&>& pTexture,
		    std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Pyramid(const glm::vec4& pColor);

};

class Quad : public Primitive
{
public:
	Quad(const std::pair<Texture2&, Texture2&>& pTexture, 
		 std::pair<uint32_t, uint32_t> pSlots);
	Quad(Texture2& pTexture, uint32_t pSlot, bool pWithTangent = false);
	Quad(const std::pair<Texture2&, Texture2&>& pTexture,
	     std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Quad(const glm::vec4& pColor);
};

class Cube : public Primitive
{
public:
	Cube(const std::pair<Texture2&, Texture2&>& pTexture,
		 std::pair<uint32_t, uint32_t> pSlots, bool pForSkybox);
	Cube(Texture2& pTexture, uint32_t pSlot, bool pForSkybox, bool pWithTangent = false);
	Cube(const std::pair<Texture2&, Texture2&>& pTexture,
		 std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor);
	Cube(const glm::vec4& pColor);

};

class Sphere : public Primitive
{
public:
	Sphere(const std::pair<Texture2&, Texture2&>& pTexture, 
		   std::pair<uint32_t, uint32_t> pSlots,
		   float pRadius, int32_t pSegments, int32_t pRings);
	Sphere(Texture2& pTexture, uint32_t pSlot, 
		   float pRadius, int32_t pSegments, int32_t pRings);
	Sphere(const std::pair<Texture2&, Texture2&>& pTexture, 
		   std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor,
		   float pRadius, int32_t pSegments, int32_t pRings);
	Sphere(const glm::vec4& pColor, float pRadius, int32_t pSegments, int32_t pRings);

};