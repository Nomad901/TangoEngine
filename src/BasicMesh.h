#pragma once
#include <concepts>

#include "Log.h"
#include "Shader.h"
#include "VertexAOS.h"
#include "VertexSOA.h"

// The difference between Mesh.h and BasicMesh.h:
// If u wanna load a model, you need to use Model class, which 
// contains Mesh class. So, basically Model class accepts only obj files,
// but can also accepts other files through assimp (but obj is faster).
// This class - can accept any type of model, it will convert the model in place
// without calling other classes and also it will load materials. 
// Basically this is almost like skinnedMesh, but skinnedMesh is focused on bones;

template<typename T>
requires std::is_base_of_v<BaseVertex, T>
class BasicMesh
{
public:


private:
	VertexType mVertex;

};

