#pragma once
#include "Includes.h"

class Program;

class Initializer
{
public:
	Initializer(Program* pProgram);

	void initAll();
	void initShaders();
	void initTextures();
	void initPrimitives();
	void initMeshes();
	void initMaterial();
	void initModels();
	void initLights();
	void initCrosshair();
	void initMousePicker();
	void initSkybox();
	void initUBO();
private:
	Program* mProgram;
};

