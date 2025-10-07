#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <thread>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"

#include "VBO.h"
#include "dependencies/rapidobj.hpp"

class OBJLoader
{
public:
	OBJLoader() = default;
	OBJLoader(const std::filesystem::path& pPath);

	std::vector<Vertex> loadOBJ(const std::filesystem::path& pPath);

	std::vector<Vertex> getVertices() const noexcept;
	
private:
	std::vector<Vertex> mVertices;

};
