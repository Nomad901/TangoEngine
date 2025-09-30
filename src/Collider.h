#pragma once
#include <iostream>
#include <format>
#include <unordered_map>

#include "glm.hpp"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "Model.h"
#include "Mesh.h"

class Collider
{
public:
	Collider() = default; 

	bool areCollided(const Mesh& pMesh1, const Mesh& pMesh2);
	bool areCollided(const Model& pModel1, const Model& pModel2);
	glm::vec3 getMinPoint(const glm::vec3& pPos, const glm::vec3& pSize);
	glm::vec3 getMaxPoint(const glm::vec3& pPos, const glm::vec3& pSize);

};

