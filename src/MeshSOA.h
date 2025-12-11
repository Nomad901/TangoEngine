#pragma once
#include "VertexSOA.h"
#include "BasicMesh.h"

class MeshSOA : public BasicMesh
{
public:
	MeshSOA() = default;
	
	void loadMesh(const std::filesystem::path& pPath) override;
	void destroy() override;

	uint32_t getNumberOfVertices() const noexcept override;

	vertexFormat getVertexFormat() const noexcept override;

	void render() override;

private:
	VertexSOA mVertexType;
	
	VAO mVAO;
	VBO mVBO;
	VBOLayout mVBOLayout;
	EBO mEBO;
};

