#include "VBO.h"
#include "Terrain.h"

VBO::VBO(const std::vector<Vertex>& pVertices, GLenum pUsage)
{
	init(pVertices, pUsage);
}

VBO::VBO(const void* pData, GLuint pSize, GLenum pUsage)
{
	init(pData, pSize, pUsage);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &mRendererID);
}

void VBO::init(const std::vector<Vertex>& pVertices, GLenum pUsage)
{
	glGenBuffers(1, &mRendererID);
	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ARRAY_BUFFER, pVertices.size() * sizeof(Vertex), pVertices.data(), pUsage);
	//glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	//glNamedBufferData(GL_ARRAY_BUFFER, pVertices.size() * sizeof(Vertex), pVertices.data(), pUsage);
}

void VBO::init(const void* pData, GLuint pSize, GLenum pUsage)
{
	glGenBuffers(1, &mRendererID);
	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ARRAY_BUFFER, pSize, pData, pUsage);
	//glNamedBufferData(GL_ARRAY_BUFFER, pSize, pData, pUsage);
}

void VBO::destroy()
{
	glDeleteBuffers(1, &mRendererID);
}

void VBO::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void VBO::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t VBO::getID() const noexcept
{
	return mRendererID;
}

void Vertex::initVertex(const Terrain* pTerrain, int32_t pX, int32_t pZ)
{ 
	float y = pTerrain->getHeight(pX, pZ);
	float worldScale = pTerrain->getWorldScale();
	mPos = glm::vec3(pX * worldScale, y, pZ * worldScale);
	
	float size = (float)pTerrain->getTerrainSize();
	float textureScale = pTerrain->getTextureScale();
	mTexCoord = glm::vec2(textureScale * (float)pX / size, textureScale * (float)pZ / size);

	mLightFactor = pTerrain->getSlopeLight(pX, pZ);
}