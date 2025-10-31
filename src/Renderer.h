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
	void pointLightPass(GBuffer* pBuffer);
	void directionalLightPass(GBuffer* pBuffer);
	void spotLightPass(GBuffer* pBuffer);

	float calcPointLightSphere(const glm::vec3& pLightPos);
	void renderLightSphere();

private:
	SceneManager* mSceneManager;
};

