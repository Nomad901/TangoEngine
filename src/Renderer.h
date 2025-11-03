#pragma once
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

	void geometryPass(GBuffer* pBuffer);
	void beginLightPass(GBuffer* pBuffer);
	void pointLightPass();
	void directionalLightPass();
	void spotLightPass();
	void renderCubeLights();

private:
	SceneManager* mSceneManager;
};

