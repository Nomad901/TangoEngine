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

	void geometryPass(GBuffer* pGBuffer);
	void beginLightPass(GBuffer* pGBuffer);
	void pointLightPass(GBuffer* pGBuffer, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> pStorages, uint32_t pIndex);
	void directionalLightPass(GBuffer* pGBuffer);
	void stencilPass(GBuffer* pGBuffer, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> pStorages,
					 uint32_t pIndex);
	void finalPass(GBuffer* pGBuffer);
	void spotLightPass();
	void renderCubeLights();

private:
	SceneManager* mSceneManager;
};

