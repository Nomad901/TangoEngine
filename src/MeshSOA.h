#pragma once
#include <array>

#include "VertexSOA.h"
#include "BasicMesh.h"

class MeshSOA : public BasicMesh
{
private:
	enum class BUFFER_TYPE : uint32_t;
	enum class INDEX_TYPE : uint32_t;
public:
	MeshSOA() = default;
	
	void loadMesh(const std::filesystem::path& pPath) override;
	void destroy() override;

	uint32_t getNumberOfVertices() const noexcept override;

	vertexFormat getVertexFormat() const noexcept override;

	void render() override;

private:
	uint32_t getIndexBufferType(BUFFER_TYPE pBUFFER_TYPE) const noexcept;
	uint32_t getIndexOfIndexType(INDEX_TYPE pINDEX_TYPE) const noexcept;

private:
	enum class BUFFER_TYPE : uint32_t 
	{
		POS_BUFFER = 0,
		NORMAL_BUFFER = 1,
		TEX_COORD_BUFFER = 2,
		TANGENT_BUFFER = 3,
		BITANGENT_BUFFER = 4,
		INDEX_BUFFER = 5,
		TOTAL_NUM_OF_BUFFERS = 6
	};
	enum class INDEX_TYPE : uint32_t
	{
		TRANSLATION_IND = 0,
		ROTATION_IND = 1,
		SCALING_IND = 2
	};
	
	VAO mVAO;
	std::array<VBO, static_cast<uint32_t>(BUFFER_TYPE::TOTAL_NUM_OF_BUFFERS)> mBuffers;

	VertexSOA mVertexSOA;
};

