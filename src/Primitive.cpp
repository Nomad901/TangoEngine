#include "Primitive.h"

void Primitive::setVertexStrg(const std::vector<Vertex>& pVertexStrg)
{
	mVertexStrg = pVertexStrg;
}

void Primitive::setVertexWithTangentStrg(const std::vector<VertexWithTangent>& pVertexWithTangentStrg)
{
	mVertexWithTangentStrg = pVertexWithTangentStrg;
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

const std::vector<VertexWithTangent>& Primitive::getVerticesWithTangent(const std::vector<Primitive::vertexContainerForTangentVertices>& pVertices)
{
	std::vector<VertexWithTangent> verticesWithTangent(pVertices.size());
	for (size_t i = 0; i < pVertices.size(); ++i)
	{
		verticesWithTangent[i].mPos = pVertices[i].mPos;
		verticesWithTangent[i].mNormal = pVertices[i].mNormal;
		verticesWithTangent[i].mTexCoord = pVertices[i].mUV;
	}

	if (verticesWithTangent.size() == 3)
	{
		VertexWithTangent::calculateTangAndBitanForTriangle(verticesWithTangent[0],
			verticesWithTangent[1],
			verticesWithTangent[2]);
		mCachedResult = std::move(verticesWithTangent);
		return mCachedResult;
	}

	if (verticesWithTangent.size() == 4)
	{
		return getVerticesWithTangentForQuad(pVertices);
	}

	std::vector<glm::vec3> tangentSumStorage(verticesWithTangent.size(), glm::vec3(0.0f));
	std::vector<glm::vec3> bitangentSumStorage(verticesWithTangent.size(), glm::vec3(0.0f));
	std::vector<uint32_t> countTangentAndBitangent(verticesWithTangent.size(), 0);

	for (size_t i = 1; i < verticesWithTangent.size() - 1; ++i)
	{
		VertexWithTangent vertex1 = verticesWithTangent[0];
		VertexWithTangent vertex2 = verticesWithTangent[i];
		VertexWithTangent vertex3 = verticesWithTangent[i + 1];

		VertexWithTangent::calculateTangAndBitanForTriangle(vertex1, vertex2, vertex3);

		tangentSumStorage[0]   += vertex1.mTangent;
		bitangentSumStorage[0] += vertex1.mBitangent;
		countTangentAndBitangent[0]++;

		tangentSumStorage[i] += vertex2.mTangent;
		bitangentSumStorage[i] += vertex2.mBitangent;
		countTangentAndBitangent[i]++;

		tangentSumStorage[i + 1] += vertex3.mTangent;
		bitangentSumStorage[i + 1] += vertex3.mBitangent;
		countTangentAndBitangent[i + 1]++;
	}

	for (size_t i = 0; i < verticesWithTangent.size(); ++i)
	{
		if (countTangentAndBitangent[i] > 0)
		{
			verticesWithTangent[i].mTangent   = glm::normalize(tangentSumStorage[i] / static_cast<float>(countTangentAndBitangent[i]));
			verticesWithTangent[i].mBitangent = glm::normalize(bitangentSumStorage[i] / static_cast<float>(countTangentAndBitangent[i]));
		}
	}

	mCachedResult = std::move(verticesWithTangent);
	return mCachedResult;
}

const std::vector<VertexWithTangent>& Primitive::getVerticesWithTangentForQuad(const std::vector<vertexContainerForTangentVertices>& pVertices) noexcept
{
	std::vector<VertexWithTangent> verticesWithTangent;
	verticesWithTangent.reserve(4);

	VertexWithTangent vertex1, vertex2, vertex3, vertex4;
	vertex1.mPos      = pVertices[0].mPos;
	vertex1.mNormal   = pVertices[0].mNormal;
	vertex1.mTexCoord = pVertices[0].mUV;

	vertex2.mPos	  = pVertices[1].mPos;
	vertex2.mNormal   = pVertices[1].mNormal;
	vertex2.mTexCoord = pVertices[1].mUV;

	vertex3.mPos	  = pVertices[2].mPos;
	vertex3.mNormal	  = pVertices[2].mNormal;
	vertex3.mTexCoord = pVertices[2].mUV;

	vertex4.mPos	  = pVertices[3].mPos;
	vertex4.mNormal	  = pVertices[3].mNormal;
	vertex4.mTexCoord = pVertices[3].mUV;
	
	glm::vec3 tangent1, tangent2, bitangent1, bitangent2;

	{
		VertexWithTangent vertex1TMP = vertex1, vertex2TMP = vertex2, vertex3TMP = vertex3;
		VertexWithTangent::calculateTangAndBitanForTriangle(vertex1TMP, vertex2TMP, vertex3TMP);
		tangent1 = vertex1TMP.mTangent;
		bitangent1 = vertex1TMP.mBitangent;
	}

	{
		VertexWithTangent vertex2TMP = vertex2, vertex3TMP = vertex3, vertex4TMP = vertex4;
		VertexWithTangent::calculateTangAndBitanForTriangle(vertex2TMP, vertex3TMP, vertex4TMP);
		tangent2 = vertex2TMP.mTangent;
		bitangent2 = vertex2TMP.mBitangent;
	}

	vertex1.mTangent = tangent1;
	vertex1.mBitangent = bitangent1;
	vertex4.mTangent = tangent2;
	vertex4.mBitangent = bitangent2;

	vertex2.mTangent = glm::normalize(tangent1 + tangent2);
	vertex2.mBitangent = glm::normalize(bitangent1 + bitangent2);

	vertex3.mTangent = glm::normalize(tangent1 + tangent2);
	vertex3.mBitangent = glm::normalize(bitangent1 + bitangent2);

	verticesWithTangent.push_back(vertex1);
	verticesWithTangent.push_back(vertex2);
	verticesWithTangent.push_back(vertex3);
	verticesWithTangent.push_back(vertex4);

	return verticesWithTangent;
}

std::vector<Vertex>& Primitive::getVertexStrg() noexcept
{
	return mVertexStrg;
}

std::vector<VertexWithTangent>& Primitive::getVertexWithTangentStrg() noexcept
{
	return mVertexWithTangentStrg;
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

Triangle::Triangle(Texture2& pTexture, uint32_t pSlot, bool pWithTangent)
{
	std::vector<Vertex> vertices;
	vertices.reserve(3);
	std::vector<VertexWithTangent> verticesWithTangent;
	std::vector<uint32_t> indices = { 0, 1, 2 };
	setTexture(pTexture);

	if (!pWithTangent)
	{
		vertices =
		{
			{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(0.5f, 1.0f)}, 
		};
	}
	else
	{
		std::vector<vertexContainerForTangentVertices> vertexContainer;
		vertexContainer.reserve(3);

		vertexContainerForTangentVertices vertex1, vertex2, vertex3;
		vertex1.mPos = glm::vec3(-0.5f, -0.5f, 0.0f);
		vertex1.mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex1.mUV = glm::vec2(0.0f, 0.0f);

		vertex2.mPos = glm::vec3(0.5f, -0.5f, 0.0f);
		vertex2.mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex2.mUV = glm::vec2(1.0f, 0.0f);

		vertex3.mPos = glm::vec3(0.0f, 0.5f, 0.0f);  
		vertex3.mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex3.mUV = glm::vec2(0.5f, 1.0f);  

		vertexContainer.push_back(vertex1);
		vertexContainer.push_back(vertex2);
		vertexContainer.push_back(vertex3);

		verticesWithTangent = getVerticesWithTangent(vertexContainer);
		// TODO: You're storing result by VALUE, but getVerticesWithTangent returns REFERENCE!
		// This might cause issues if mCachedResult is modified later
	}

	indices = { 0, 1, 2 };

	setTexSlot(pSlot);
	if (!pWithTangent)
		setVertexStrg(vertices);
	else
		setVertexWithTangentStrg(verticesWithTangent);
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

Pyramid::Pyramid(Texture2& pTexture, uint32_t pSlot, bool pWithTangent)
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

Quad::Quad(Texture2& pTexture, uint32_t pSlot, bool pWithTangent)
{
	std::vector<Vertex> vertices;
	vertices.reserve(6);
	std::vector<VertexWithTangent> verticesWithTangent;
	verticesWithTangent.reserve(6);
	std::vector<uint32_t> indices;
	indices.reserve(6 * 2);
	setTexture(pTexture);

	if (!pWithTangent)
	{
		vertices =
		{
			{glm::vec3(-0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.5f, -0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(-0.5f,  0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}
		};
	}
	else
	{
		std::vector<vertexContainerForTangentVertices> vertexContainer;
		vertexContainer.reserve(4);
		vertexContainerForTangentVertices vertex1;
		vertex1.mPos	= glm::vec3(-0.5f, -0.5f, 1.0f);
		vertex1.mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex1.mUV		= glm::vec2(0.0f, 0.0f);

		vertexContainerForTangentVertices vertex2;
		vertex1.mPos	= glm::vec3(0.5f, -0.5f, 1.0f);
		vertex1.mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex1.mUV		= glm::vec2(1.0f, 0.0f);

		vertexContainerForTangentVertices vertex3;
		vertex1.mPos	= glm::vec3(0.5f, 0.5f, 1.0f);
		vertex1.mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex1.mUV		= glm::vec2(1.0f, 1.0f);

		vertexContainerForTangentVertices vertex4;
		vertex1.mPos	= glm::vec3(-0.5f, 0.5f, 1.0f);
		vertex1.mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertex1.mUV		= glm::vec2(0.0f, 1.0f);

		vertexContainer.push_back(vertex1);
		vertexContainer.push_back(vertex2);
		vertexContainer.push_back(vertex3);
		vertexContainer.push_back(vertex4);

		verticesWithTangent = getVerticesWithTangentForQuad(vertexContainer);
	}
	indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	setTexSlot(pSlot);
	if (!pWithTangent)
		setVertexStrg(vertices);
	else
		setVertexWithTangentStrg(verticesWithTangent);

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

Cube::Cube(Texture2& pTexture, uint32_t pSlot, bool pForSkybox, bool pWithTangent)
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
					 std::pair<uint32_t, uint32_t> pSlots, 
			   float pRadius, int32_t pSegments, int32_t pRings)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTextures(pTexture);

	for (int32_t i = 0; i <= pRings; ++i)
	{
		float ringsAngle = glm::pi<float>() / 2 - i * (glm::pi<float>() / pRings);
		float xy = pRadius * cosf(ringsAngle);
		float z = pRadius * sinf(ringsAngle);

		for (int32_t j = 0; j <= pSegments; ++j)
		{
			float segmentAngle = j * (2 * glm::pi<float>() / pSegments);

			Vertex vertex;
			vertex.mPos.x = xy * cosf(segmentAngle);
			vertex.mPos.y = xy * sinf(segmentAngle);
			vertex.mPos.z = z;

			vertex.mNormals = glm::normalize(vertex.mPos);

			vertex.mTexCoord.x = (float)j / pSegments;
			vertex.mTexCoord.y = (float)i / pRings;

			vertex.mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			vertices.push_back(vertex);
		}
	}

	for (int32_t i = 0; i < pRings; ++i)
	{
		for (int32_t j = 0; j < pSegments; ++j)
		{
			int32_t first = (i * (pSegments + 1)) + j;
			int32_t second = first + pSegments + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);
			indices.push_back(first + 1);
			indices.push_back(second);
			indices.push_back(second + 1);
		}
	}

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Sphere::Sphere(Texture2& pTexture, uint32_t pSlot, 
			   float pRadius, int32_t pSegments, int32_t pRings)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTexture(pTexture);

	for (int32_t i = 0; i <= pRings; ++i)
	{
		float ringsAngle = glm::pi<float>() / 2 - i * (glm::pi<float>() / pRings);
		float xy = pRadius * cosf(ringsAngle);
		float z = pRadius * sinf(ringsAngle);

		for (int32_t j = 0; j <= pSegments; ++j)
		{
			float segmentAngle = j * (2 * glm::pi<float>() / pSegments);

			Vertex vertex;
			vertex.mPos.x = xy * cosf(segmentAngle);
			vertex.mPos.y = xy * sinf(segmentAngle);
			vertex.mPos.z = z;

			vertex.mNormals = glm::normalize(vertex.mPos);

			vertex.mTexCoord.x = (float)j / pSegments;
			vertex.mTexCoord.y = (float)i / pRings;

			vertex.mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			vertices.push_back(vertex);
		}
	}

	for (int32_t i = 0; i < pRings; ++i)
	{
		for (int32_t j = 0; j < pSegments; ++j)
		{
			int32_t first = (i * (pSegments + 1)) + j;
			int32_t second = first + pSegments + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);
			indices.push_back(first + 1);
			indices.push_back(second);
			indices.push_back(second + 1);
		}
	}

	setTexSlot(pSlot);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Sphere::Sphere(const std::pair<Texture2&, Texture2&>& pTexture,
					 std::pair<uint32_t, uint32_t> pSlots, const glm::vec4& pColor,
			   float pRadius, int32_t pSegments, int32_t pRings)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(36);
	setTextures(pTexture);

	for (int32_t i = 0; i <= pRings; ++i)
	{
		float ringsAngle = glm::pi<float>() / 2 - i * (glm::pi<float>() / pRings);
		float xy = pRadius * cosf(ringsAngle);
		float z = pRadius * sinf(ringsAngle);

		for (int32_t j = 0; j <= pSegments; ++j)
		{
			float segmentAngle = j * (2 * glm::pi<float>() / pSegments);

			Vertex vertex;
			vertex.mPos.x = xy * cosf(segmentAngle);
			vertex.mPos.y = xy * sinf(segmentAngle);
			vertex.mPos.z = z;

			vertex.mNormals = glm::normalize(vertex.mPos);

			vertex.mTexCoord.x = (float)j / pSegments;
			vertex.mTexCoord.y = (float)i / pRings;

			vertex.mColor = pColor;

			vertices.push_back(vertex);
		}
	}

	for (int32_t i = 0; i < pRings; ++i)
	{
		for (int32_t j = 0; j < pSegments; ++j)
		{
			int32_t first = (i * (pSegments + 1)) + j;
			int32_t second = first + pSegments + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);
			indices.push_back(first + 1);
			indices.push_back(second);
			indices.push_back(second + 1);
		}
	}

	setTexSlots(pSlots);
	setVertexStrg(vertices);
	setIndexStrg(indices);
}

Sphere::Sphere(const glm::vec4& pColor, float pRadius, int32_t pSegments, int32_t pRings)
{
	std::vector<Vertex> vertices;
	vertices.reserve((pSegments + 1) * (pRings + 1));
	std::vector<uint32_t> indices;
	indices.reserve((pSegments + 1) * (pRings + 1) * 6);
	
	for (int32_t i = 0; i <= pRings; ++i)
	{
		float ringsAngle = glm::pi<float>() / 2 - i * (glm::pi<float>() / pRings);
		float xy = pRadius * cosf(ringsAngle);
		float z = pRadius * sinf(ringsAngle);

		for (int32_t j = 0; j <= pSegments; ++j)
		{
			float segmentAngle = j * (2 * glm::pi<float>() / pSegments);
			
			Vertex vertex;
			vertex.mPos.x = xy * cosf(segmentAngle);
			vertex.mPos.y = xy * sinf(segmentAngle);
			vertex.mPos.z = z;

			vertex.mNormals = glm::normalize(vertex.mPos);

			vertex.mTexCoord.x = (float)j / pSegments;
			vertex.mTexCoord.y = (float)i / pRings;

			vertex.mColor = pColor;

			vertices.push_back(vertex);
		}
	}

	for (int32_t i = 0; i < pRings; ++i)
	{
		for (int32_t j = 0; j < pSegments; ++j)
		{
			int32_t first  = (i * (pSegments + 1)) + j;
			int32_t second = first + pSegments + 1;

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