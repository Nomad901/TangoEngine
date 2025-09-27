#pragma once
#include "Includes.h"
#include "SceneManager.h"

class Renderer
{
public:
	Renderer(SceneManager* pSceneManager);
	
	void preDrawScene();
	void drawScene();
	void showFPS();

private:
	void takeCursor();

	void setImGui();
	void setGLproperties();

private:
	SceneManager* mSceneManager;
};

