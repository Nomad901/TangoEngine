#pragma once
#include <iostream>
#include <unordered_map>

#include "SDL3/SDL.h"

#include "SceneManager.h"
#include "Player.h"
#include "TeleportSystem.h"

class Controler
{
public:
	Controler(SceneManager* pSceneManager);

	void controlAll(float pDeltaTime);
	void controlScreen();
	void controlCamera(float pDelaTime);
	void controlModel();
	void controlLight();

	Player& getPlayer() noexcept;

private:
	SceneManager* mSceneManager;
	Player mPlayer;
	TeleportSystem mTeleportSystem;
	std::unordered_map<SDL_Keycode, bool> mKeyCodes;
	SDL_Event mEvent;
};

