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

	std::ifstream ifstream(pPath);
	if (!ifstream.is_open())
		return {};

	std::string line;
	glm::vec3 tmpVec3;
	glm::vec2 tmpVec2;

	line.reserve(256);
	positions.reserve(10000);
	normals.reserve(10000);
	texPos.reserve(10000);
	mVertices.reserve(30000);

	while (std::getline(ifstream, line))
	{
		const char* strView = line.data();
		if (strView[0] == 'v')
		{
			if (strView[1] == ' ')
			{
				sscanf_s(strView + 2, "%f %f %f", &tmpVec3.x, &tmpVec3.y, &tmpVec3.z);
				positions.emplace_back(tmpVec3);
			}
			else if (strView[1] == 't')
			{
				sscanf_s(strView + 3, "%f %f", &tmpVec2.x, &tmpVec2.y);
				texPos.emplace_back(tmpVec2);
			}
			else if (strView[1] == 'n')
			{
				sscanf_s(strView + 3, "%f %f %f", &tmpVec3.x, &tmpVec3.y, &tmpVec3.z);
				normals.emplace_back(tmpVec3);
			}
		}
		else if (strView[0] == 'f')
		{
			strView += 2;
			int32_t v, vt, vn;
			while (*strView)
			{
				int count = sscanf_s(strView, "%d/%d/%d", &v, &vt, &vn);
				if (count == 3)
				{
					mVertices.emplace_back(
						positions[v - 1],
						normals[vn - 1],
						glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
						texPos[vt - 1]
					);
				}

				while (*strView && *strView != ' ') strView++;
				while (*strView == ' ') strView++;
			}
		}
	}

	return mVertices;
}

std::vector<Vertex> OBJLoader::getVertices() const noexcept
{
	return mVertices;
}
