#pragma once
#include <iostream>
#include <format>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "SDL3/SDL.h"
#include "glm.hpp"

class Program;

class UI
{
public:
	UI() = default;
	UI(SDL_Window* pWindow, const SDL_GLContext& pContext);
	~UI();

	void init(SDL_Window* pWindow, const SDL_GLContext& pContext);

	void control(Program& pProgram);
};

