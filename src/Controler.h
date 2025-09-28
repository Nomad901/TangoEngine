#pragma once
#include "Includes.h"
#include "SceneManager.h"

class Controler
{
public:
	Controler(SceneManager* pSceneManager);

	void controlAll();
	void controlScreen();
	void controlCamera();
	void controlModel();
	void controlLight();

	Player& getPlayer() noexcept;

private:
	SceneManager* mSceneManager;
	Player mPlayer;
	std::unordered_map<SDL_Keycode, bool> mKeyCodes;
	SDL_Event mEvent;
};

