#include "OBJLoader.h"

OBJLoader::OBJLoader(const std::filesystem::path& pPath)
{
	loadOBJ(pPath);
}

std::vector<Vertex> OBJLoader::loadOBJ(const std::filesystem::path& pPath)
{
	std::vector<glm::fvec3> vertexPositions;
	std::vector<glm::fvec3> vertexNormals;
	std::vector<glm::fvec2> vertexTexPos;

	std::stringstream ss;
	std::ifstream ifstream(pPath);
	std::string line = "";
	std::string prefix;
	glm::vec3 vec3 = glm::vec3(1.0f);
	glm::vec2 vec2 = glm::vec2(1.0f);

	mVertices.reserve(1000);

	if (!ifstream.is_open())
	{
		std::cout << std::format("Ifstream is not opened! File path: {}\n", pPath.string());
		return {};
	}

	while (std::getline(ifstream, line))
	{
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "v") 
		{
			ss >> vec3.x >> vec3.y >> vec3.z;
			vertexPositions.push_back(vec3);
		}
		else if (prefix == "vt")
		{
			ss >> vec2.x >> vec2.y;
			vertexTexPos.push_back(vec2);
		}
		else if (prefix == "vn")
		{
			ss >> vec3.x >> vec3.y >> vec3.z;
			vertexNormals.push_back(vec3);
		}
		else if (prefix == "f")
		{
			std::string fLine;
			while (ss >> fLine)
			{
				std::replace(fLine.begin(), fLine.end(), '/', ' ');
				std::istringstream data(fLine);

				Vertex vertex;
				int32_t vertPosInd = -1;
				int32_t normalInd  = -1;
				int32_t texPosInd  = -1;

				data >> vertPosInd;
				if (data.peek() != EOF)
					data >> texPosInd;
				if (data.peek() != EOF)
					data >> normalInd;

				if (vertPosInd != -1)
				{
					vertex.mPos = vertexPositions[vertPosInd - 1];
				}
				if (texPosInd != -1)
				{
					vertex.mTexCoord = vertexTexPos[texPosInd - 1];	
				}
				if (normalInd != -1)
				{
					vertex.mNormals = vertexNormals[normalInd - 1];
				}

				vertex.mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				mVertices.push_back(vertex);
			}
		}	
	}

	return mVertices;
}

std::vector<Vertex> OBJLoader::getVertices() const noexcept
{
	return mVertices;
}
