#pragma once
#include "Includes.h"

class Program;

struct Initializer
{
	void initAll(Program& pProgram);
	void initShaders(Program& pProgram);
	void initTextures(Program& pProgram);
	void initPrimitives(Program& pProgram);
	void initMeshes(Program& pProgram);
	void initMaterial(Program& pProgram);
	void initModels(Program& pProgram);
	void initLights(Program& pProgram);
	void initCrosshair(Program& pProgram);
	void initMousePicker(Program& pProgram);
	void initSkybox(Program& pProgram);
	void initUBO(Program& pProgram);
};

