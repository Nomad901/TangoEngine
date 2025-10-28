#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <string>
#include <unordered_map>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "SDL3/SDL.h"
#include "glm.hpp"

class SceneManager;

class UI
{
public:
	UI() = default;
	UI(SDL_Window* pWindow, const SDL_GLContext& pContext);
	~UI();

	void init(SDL_Window* pWindow, const SDL_GLContext& pContext);

	void control(SceneManager& pSceneManager);
private:
	//
	// ImGui UI
	// 
	// --------
	void createWindow(glm::vec2 pPos, glm::vec2 pSize, bool pIsCollapsed);
	void setSpaces(uint32_t pAmountOfSpaces);

	void setSliderFloat(std::string_view pName, float& pValue, float pMin, float pMax);
	//
	// if the unordered map is confusing - 
	// the map takes std::string_view. This string_view serves as a name for each column in sliderFloat2/3/4("string_view", ...);
	// std::pair<float, float> here, is just min and max value of each column. 
	// so, basically we just correlate name and min/max values together
	//
	void setSlidersVec2(glm::vec2& pVec2, const std::unordered_map<std::string_view, std::pair<float, float>>& pParam);
	void setSlidersVec3(glm::vec3& pVec3, const std::unordered_map<std::string_view, std::pair<float, float>>& pParam);
	void setSlidersVec4(glm::vec4& pVec4, const std::unordered_map<std::string_view, std::pair<float, float>>& pParam);

	void setColorVec3(std::string_view pName, glm::vec3& pVecColor);
	void setColorVec4(std::string_view pName, glm::vec4& pVecColor);
	//--------

	//
	// Controlling 
	//
	// -----------
	void manageTerrain(SceneManager& pSceneManager);
	void manageFonts(SceneManager& pSceneManager);
	void manageLight(SceneManager& pSceneManager);
	void manageAll(SceneManager& pSceneManager);
	// -----------

private:
	glm::vec2 mWindowPos{ 1.0f, 1.0f };
	glm::vec2 mWindowSize{ 400.0f, 500.0f };
	bool mWindowIsCollapsed{ true };
}; 
