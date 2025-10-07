#pragma once
#include "SceneManager.h"

class Initializer
{
public:
	Initializer(SceneManager* pSceneManager);

	void init(bool pInitAll);

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
	void initTerrain();

private:
	SceneManager* mSceneManager;
};

