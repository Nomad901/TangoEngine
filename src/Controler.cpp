#include "Controler.h"
#include "Program.h"

Controler::Controler(SceneManager* pSceneManager)
{
	mSceneManager = pSceneManager;
}

void Controler::controlAll()
{
	while (SDL_PollEvent(&mEvent))
	{
		ImGui_ImplSDL3_ProcessEvent(&mEvent);

		if (mEvent.type == SDL_EVENT_QUIT ||
			mEvent.key.key == SDLK_ESCAPE)
		{
			mSceneManager->getProgramProperties().mProgIsRunning = false;
			break;
		}

		// KEYS CHECKERS
		if (mEvent.type == SDL_EVENT_KEY_DOWN)
			mKeyCodes[mEvent.key.key] = true;
		if (mEvent.type == SDL_EVENT_KEY_UP)
			mKeyCodes[mEvent.key.key] = false;

		// CAMERA MOVING
		if (mEvent.type == SDL_EVENT_MOUSE_MOTION && mSceneManager->getProgramProperties().mTakeCursor)
			mSceneManager->getProgramProperties().mCamera.mouseMovement(glm::vec2(mEvent.motion.xrel, mEvent.motion.yrel));

		// LIGHT BLOCK DISTANCE FROM CAMERA	
		if (mEvent.type == SDL_EVENT_MOUSE_WHEEL) {
			if (mEvent.wheel.y > 0)
				mSceneManager->getProgramProperties().mRadius += 0.5f;
			if (mEvent.wheel.y < 0)
				mSceneManager->getProgramProperties().mRadius -= 0.5f;
		}
	}

	controlScreen();
	controlCamera();
	controlModel();
	//controlLight();
}

void Controler::controlScreen()
{
	// WHITE/BLACK SCREEN
	if (mKeyCodes[SDLK_V])
		mSceneManager->getProgramProperties().mWhiteScreen = true;
	if (mKeyCodes[SDLK_B])
		mSceneManager->getProgramProperties().mWhiteScreen = false;

	// CURSOR
	if (mKeyCodes[SDLK_Z])
		mSceneManager->getProgramProperties().mTakeCursor = !mSceneManager->getProgramProperties().mTakeCursor;
}

void Controler::controlCamera()
{
	float speed = 1.0f;
	if (mKeyCodes[SDLK_LSHIFT])
		speed = 2.0f;
	if (mKeyCodes[SDLK_W])
		mSceneManager->getProgramProperties().mCamera.moveCamera(moveSides::FORWARD, speed);
	if (mKeyCodes[SDLK_S])
		mSceneManager->getProgramProperties().mCamera.moveCamera(moveSides::BACKWARD, speed);
	if (mKeyCodes[SDLK_A])
		mSceneManager->getProgramProperties().mCamera.moveCamera(moveSides::LEFT, speed);
	if (mKeyCodes[SDLK_D])
		mSceneManager->getProgramProperties().mCamera.moveCamera(moveSides::RIGHT, speed);
	if (mKeyCodes[SDLK_LCTRL])
		mSceneManager->getProgramProperties().mCamera.moveCamera(moveSides::DOWN, speed);
	if (mKeyCodes[SDLK_SPACE])
		mSceneManager->getProgramProperties().mCamera.moveCamera(moveSides::UP, speed);
}

void Controler::controlModel()
{	
	// ROTATE DEGREE
	if (mKeyCodes[SDLK_R])
		mSceneManager->getModelProperties().mRotateDegree -= 0.4f;
	if (mKeyCodes[SDLK_F])
		mSceneManager->getModelProperties().mRotateDegree += 0.4f;
}

void Controler::controlLight()
{	

}
