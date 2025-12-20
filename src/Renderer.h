#pragma once
#include "SceneManager.h"
#include "HDRManager.h"

class Controler;

class Renderer
{
public:
	Renderer(SceneManager* pSceneManager);
	
	void preDrawScene();
	void drawScene(Controler* pControler);
	void showFPS();

private:
	void takeCursor();
	
	void setImGui();
	void setGLproperties();

	void drawSceneTMP();
	void renderCubeLights();

private:
	SceneManager* mSceneManager;
};

