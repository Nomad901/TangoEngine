#include "OBJLoader.h"

OBJLoader::OBJLoader(const std::filesystem::path& pPath)
{
	loadOBJ(pPath);
}

std::vector<Vertex> OBJLoader::loadOBJ(const std::filesystem::path& pPath)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texPos;
	std::vector<glm::vec4> colors;
	colors.reserve(10000);
	positions.reserve(10000);
	normals.reserve(10000);
	texPos.reserve(10000);
	mVertices.reserve(30000);

	rapidobj::Result result = rapidobj::ParseFile(pPath);


	return mVertices;
}

std::vector<Vertex> OBJLoader::getVertices() const noexcept
{
	return mVertices;
}
