#include "Controler.h"
#include "Program.h"

Controler::Controler(SceneManager* pSceneManager)
{
	mSceneManager = pSceneManager;
	mPlayer.init(glm::vec3(1.0f, 16.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f, 100.0f, 30.0f, true, 
				 pSceneManager->getProgramProperties().mResourcePath + "Models/player.obj");
}

void Controler::controlAll(float pDeltaTime)
{
	if (mPlayer.isInThirdPersonCamera())
	{
		mSceneManager->getProgramProperties().mViewMatrix = mPlayer.getThirdPersonCamera().getViewMatrix();
		mSceneManager->getProgramProperties().mThirdPersonCam = mPlayer.getThirdPersonCamera();
	}
	else
	{
		mSceneManager->getProgramProperties().mViewMatrix = mPlayer.getCamera().getViewMatrix();
		mSceneManager->getProgramProperties().mCamera = mPlayer.getCamera();
	}
	
	mPlayer.turnOnNoclip(mSceneManager->getProgramProperties().mNoclip);

	while (SDL_PollEvent(&mEvent))
	{
		ImGui_ImplSDL3_ProcessEvent(&mEvent);
		mPlayer.updateEvents(mEvent);

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
		//if (mEvent.type == SDL_EVENT_MOUSE_MOTION && mSceneManager->getProgramProperties().mTakeCursor)
		//	mPlayer.getThirdPersonCamera().mouseMovement(glm::vec2(mEvent.motion.xrel, mEvent.motion.yrel));

		// LIGHT BLOCK DISTANCE FROM CAMERA	
		if (mEvent.type == SDL_EVENT_MOUSE_WHEEL) {
			if (mEvent.wheel.y > 0)
				mSceneManager->getProgramProperties().mRadius += 0.5f;
			if (mEvent.wheel.y < 0)
				mSceneManager->getProgramProperties().mRadius -= 0.5f;
		}
	}

	if (mKeyCodes[SDLK_E])
	{
		glm::vec3 mPos = mPlayer.getPos();
		std::cout << std::format("Character pos: {}/{}/{}\n", mPos.x, mPos.y, mPos.z);
	}

	controlScreen();
	controlCamera(pDeltaTime);
	controlModel();
	//controlLight();

	mTeleportSystem.update(mPlayer);	
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

void Controler::controlCamera(float pDeltaTime)
{
	mPlayer.sprint(mKeyCodes[SDLK_LSHIFT]);
	if (mKeyCodes[SDLK_W])
		mPlayer.move(moveSidesPlayer::FORWARD, pDeltaTime);
	if (mKeyCodes[SDLK_S])
		mPlayer.move(moveSidesPlayer::BACKWARD, pDeltaTime);
	if (mKeyCodes[SDLK_A])
		mPlayer.move(moveSidesPlayer::LEFT, pDeltaTime);
	if (mKeyCodes[SDLK_D])
		mPlayer.move(moveSidesPlayer::RIGHT, pDeltaTime);
	if (mKeyCodes[SDLK_SPACE])
		mPlayer.jump(); 
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

Player& Controler::getPlayer() noexcept
{
	return mPlayer;
}

SDL_Event& Controler::getEvents() noexcept
{
	return mEvent;
}
