#include "Primitive.h"

void Primitive::setVertexStrg(const std::vector<Vertex>& pVertexStrg)
{
	mVertexStrg = pVertexStrg;
}

void Primitive::setIndexStrg(const std::vector<uint32_t>& pIndexStrg)
{
	mIndexStrg = pIndexStrg;
}

void Primitive::setTexture(Texture2& pTexture)
{
	mTexture = std::move(pTexture);
}

void Primitive::setTextures(const std::pair<Texture2&, Texture2&>& pTexture)
{
	mTextures.first = std::move(pTexture.first);
	mTextures.second = std::move(pTexture.second);
}

void Primitive::setTexSlots(std::pair<uint32_t, uint32_t> pTexSlots)
{
	mTexSlots = pTexSlots;
}

void Primitive::setTexSlot(uint32_t pSlot)
{
	mSlot = pSlot;
}

std::pair<uint32_t, uint32_t> Primitive::getTexSlots() const noexcept
{
	return mTexSlots;
}

uint32_t Primitive::getSingleSlot() const noexcept
{
	return mSlot;
}

Texture2& Primitive::getFirstTex() noexcept
{
	return mTextures.first;
}

Texture2& Primitive::getSecondTex() noexcept
{
	return mTextures.second;
}

Texture2& Primitive::getSingleTex() noexcept
{
	return mTexture;
}

std::vector<Vertex>& Primitive::getVertexStrg() noexcept
{
	return mVertexStrg;
}

std::vector<uint32_t>& Primitive::getIndexStrg() noexcept
{
	return mIndexStrg;
}

Triangle::Triangle(const std::pair<Texture2&, Texture2&>& pTexture,
				   std::pair<uint32_t, uint32_t> pSlots)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTextures(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3( 0.0f,  0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	};
	indices =
	{
		0, 1, 2
	};

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Triangle::Triangle(Texture2& pTexture, uint32_t pSlot)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTexture(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.0f,  0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	};
	indices =
	{
		0, 1, 2
	};

	setTexSlot(pSlot);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Triangle::Triangle(const std::pair<Texture2&, Texture2&>& pTexture,
				   std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTextures(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.0f,  0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  pColor, glm::vec2(1.0f, 1.0f)},
	};
	indices =
	{
		0, 1, 2
	};

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Triangle::Triangle(const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.0f,  0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  pColor, glm::vec2(1.0f, 1.0f)},
	};
	indices =
	{
		0, 1, 2
	};

	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Pyramid::Pyramid(const std::pair<Texture2&, Texture2&>& pTexture,
				 std::pair<uint32_t, uint32_t> pSlots)
{
	std::vector<Vertex> vertices;
	vertices.reserve(16); 
	std::vector<uint32_t> indices;
	indices.reserve(18); 
	setTextures(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}, 
		{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}, 
		{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}, 
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}, 
		{glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)},   

		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}, 
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},  
		{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},  
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}, 
		{glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)},   

		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}, 
		{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}, 
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)},  

		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}, 
		{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}, 
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)},  
	};

	indices =
	{
		0, 1, 2,
		2, 3, 0,
		3, 2, 4,
		1, 0, 4,
		0, 3, 4,
		2, 1, 4
	};

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Pyramid::Pyramid(Texture2& pTexture, uint32_t pSlot)
{
	std::vector<Vertex> vertices;
	vertices.reserve(16);
	std::vector<uint32_t> indices;
	indices.reserve(18);
	setTexture(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)},

		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)},

		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)},

		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)},
	};

	indices =
	{
		0, 1, 2,
		2, 3, 0,
		3, 2, 4,
		1, 0, 4,
		0, 3, 4,
		2, 1, 4
	};

	setTexSlot(pSlot);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Pyramid::Pyramid(const std::pair<Texture2&, Texture2&>& pTexture,
				 std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(16);
	std::vector<uint32_t> indices;
	indices.reserve(18);
	setTextures(pTexture);

	vertices =
	{
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   pColor, glm::vec2(0.5f, 1.0f) },
		
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   pColor, glm::vec2(0.5f, 1.0f) },
		
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   pColor, glm::vec2(0.5f, 1.0f) },
		
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   pColor, glm::vec2(0.5f, 1.0f) }
	};

	indices =
	{
		0, 1, 2,
		2, 3, 0,
		3, 2, 4,
		1, 0, 4,
		0, 3, 4,
		2, 1, 4
	};

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Pyramid::Pyramid(const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(16);
	std::vector<uint32_t> indices;
	indices.reserve(18);

	vertices =
	{
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   pColor, glm::vec2(0.5f, 1.0f) },

		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   pColor, glm::vec2(0.5f, 1.0f) },

		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   pColor, glm::vec2(0.5f, 1.0f) },

		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f),  pColor, glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), pColor, glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   pColor, glm::vec2(0.5f, 1.0f) }
	};

	indices =
	{
		0, 1, 2,
		2, 3, 0,
		3, 2, 4,
		1, 0, 4,
		0, 3, 4,
		2, 1, 4
	};

	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Quad::Quad(const std::pair<Texture2&, Texture2&>& pTexture,
		   std::pair<uint32_t, uint32_t> pSlots)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTextures(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3( 0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}
	};
	indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Quad::Quad(Texture2& pTexture, uint32_t pSlot)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTexture(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}
	};
	indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	setTexSlot(pSlot);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Quad::Quad(const std::pair<Texture2&, Texture2&>& pTexture,
		   std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTextures(pTexture);
	
	vertices =
	{	
		{glm::vec3(-0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3( 0.5f,  0.5f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(0.0f, 1.0f)}
	};
	indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Quad::Quad(const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3( 0.5f,  0.5f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), pColor, glm::vec2(0.0f, 1.0f)}
	};
	indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Cube::Cube(const std::pair<Texture2&, Texture2&>& pTexture,
		   std::pair<uint32_t, uint32_t> pSlots, bool pForSkybox)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36); 
	setTextures(pTexture);

	if (!pForSkybox)
	{
		vertices =
		{
			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},

			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},

			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

			{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},

			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
		};
	}
	else
	{
		vertices = 
		{
			{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},

			{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},

			{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f,  -1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f,  -1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f,  -1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f,  -1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},

			{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},

			{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},

			{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)}
		};
	}
	indices =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Cube::Cube(Texture2& pTexture, uint32_t pSlot, bool pForSkybox)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTexture(pTexture);

	if (!pForSkybox)
	{
		vertices =
		{
			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},

			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},

			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

			{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},

			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
		};
	}
	else
	{
		vertices =
		{
			{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f,  0.0f,  -1.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},

			{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},

			{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f,  -1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f,  -1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f,  -1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f,  -1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},

			{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},

			{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},

			{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f), glm::vec2(1.0f, 0.0f)}
		};
	}
	indices =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	setTexSlot(pSlot);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Cube::Cube(const std::pair<Texture2&, Texture2&>& pTexture,
		   std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTextures(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), pColor, glm::vec2(0.0f, 1.0f)},
																		
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), pColor, glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), pColor, glm::vec2(1.0f, 1.0f)},
																		
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), pColor, glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), pColor, glm::vec2(0.0f, 0.0f)},
																		
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), pColor, glm::vec2(0.0f, 1.0f)},
																		
		{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), pColor, glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), pColor, glm::vec2(0.0f, 0.0f)},
																		
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), pColor, glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), pColor, glm::vec2(1.0f, 0.0f)}
	};

	indices =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Cube::Cube(const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), pColor, glm::vec2(0.0f, 1.0f)},
																		
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), pColor, glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), pColor, glm::vec2(1.0f, 1.0f)},
																		
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), pColor, glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), pColor, glm::vec2(0.0f, 0.0f)},
																		
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), pColor, glm::vec2(0.0f, 1.0f)},
																		
		{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), pColor, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), pColor, glm::vec2(0.0f, 1.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), pColor, glm::vec2(0.0f, 0.0f)},
																		
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), pColor, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), pColor, glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), pColor, glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), pColor, glm::vec2(1.0f, 0.0f)}
	};

	indices =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Sphere::Sphere(const std::pair<Texture2&, Texture2&>& pTexture, 
					 std::pair<uint32_t, uint32_t> pSlots)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTextures(pTexture);

	
	

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Sphere::Sphere(Texture2& pTexture, uint32_t pSlot, bool pForSkybox)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTexture(pTexture);



	setTexSlot(pSlot);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Sphere::Sphere(const std::pair<Texture2&, Texture2&>& pTexture,
					 std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTextures(pTexture);



	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Sphere::Sphere(const glm::vec4& pColor, float pRadius, int32_t pSectors, int32_t pStacks)
{
	std::vector<Vertex> vertices;
	vertices.reserve((pSectors + 1) * (pStacks + 1));
	std::vector<uint32_t> indices;
	indices.reserve((pSectors + 1) * (pStacks + 1) * 6);
	
	for (int32_t i = 0; i <= pStacks; ++i)
	{
		float stackAngle = glm::pi<float>() / 2 - i * (glm::pi<float>() / pStacks);
		float xy = pRadius * cosf(stackAngle);
		float z = pRadius * sinf(stackAngle);

		for (int32_t j = 0; j <= pSectors; ++j)
		{
			float sectorAngle = j * (2 * glm::pi<float>() / pSectors);
			
			Vertex vertex;
			vertex.mPos.x = xy * cosf(sectorAngle);
			vertex.mPos.y = xy * sinf(sectorAngle);
			vertex.mPos.z = z;

			vertex.mNormals = glm::normalize(vertex.mPos);

			vertex.mTexCoord.x = (float)j / pSectors;
			vertex.mTexCoord.y = (float)i / pStacks;

			vertex.mColor = pColor;

			vertices.push_back(vertex);
		}
	}

	for (int32_t i = 0; i < pStacks; ++i)
	{
		for (int32_t j = 0; j < pSectors; ++j)
		{
			int32_t first  = (i * (pSectors + 1)) + j;
			int32_t second = first + pSectors + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);
			indices.push_back(first + 1);
			indices.push_back(second);
			indices.push_back(second + 1);
		}
	}

	setVertexStrg(vertices);
	setIndexStrg(indices);
}
