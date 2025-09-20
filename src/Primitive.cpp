#include "Primitive.h"

void Primitive::setVertexStrg(const std::vector<Vertex>& pVertexStrg)
{
	mVertexStrg = pVertexStrg;
}

void Primitive::setIndexStrg(const std::vector<uint32_t>& pIndexStrg)
{
	mIndexStrg = pIndexStrg;
}

void Primitive::setTexture(const Texture2& pTexture)
{
	mTexture = pTexture;
}

void Primitive::setTexSloth(uint32_t pTexSloth)
{
	mTexSloth = pTexSloth;
}

uint32_t Primitive::getTexSloth() noexcept
{
	return mTexSloth;
}

std::vector<Vertex>& Primitive::getVertexStrg() noexcept
{
	return mVertexStrg;
}

std::vector<uint32_t>& Primitive::getIndexStrg() noexcept
{
	return mIndexStrg;
}

Texture2& Primitive::getTexture() noexcept
{
	return mTexture;
}

Triangle::Triangle(const Texture2& pTexture, uint32_t pSloth)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTexture(pTexture);

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

	setTexSloth(pSloth);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Triangle::Triangle(const Texture2& pTexture, uint32_t pSloth, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTexture(pTexture);

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

	setTexSloth(pSloth);
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

Pyramid::Pyramid(const Texture2& pTexture, uint32_t pSloth)
{
	std::vector<Vertex> vertices;
	vertices.reserve(16); 
	std::vector<uint32_t> indices;
	indices.reserve(18); 
	setTexture(pTexture);

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

	setTexSloth(pSloth);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Pyramid::Pyramid(const Texture2& pTexture, uint32_t pSloth, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(16);
	std::vector<uint32_t> indices;
	indices.reserve(18);
	setTexture(pTexture);

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

	setTexSloth(pSloth);
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

Quad::Quad(const Texture2& pTexture, uint32_t pSloth)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTexture(pTexture);

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

	setTexSloth(pSloth);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Quad::Quad(const Texture2& pTexture, uint32_t pSloth, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTexture(pTexture);
	
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

	setTexSloth(pSloth);
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

Cube::Cube(const Texture2& pTexture, uint32_t pSloth)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36); 
	setTexture(pTexture);

	vertices =
	{
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}, 
		{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}, 
		{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}, 
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}, 
														  
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}, 
		{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}, 
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
														  
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}, 
		{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}, 
		{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}, 
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}, 
																 
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}, 
		{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}, 
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
																 
		{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}, 
		{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}, 
		{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}, 
		{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}, 
																 
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)} 
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

	setTexSloth(pSloth);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Cube::Cube(const Texture2& pTexture, uint32_t pSloth, const glm::vec4& pColor)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTexture(pTexture);

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

	setTexSloth(pSloth);
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

	setVertexStrg(vertices);
	setIndexStrg(indices);
}
