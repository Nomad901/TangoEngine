#include "Controler.h"
#include "Program.h"

Controler::Controler(SceneManager* pSceneManager)
{
	mSceneManager = pSceneManager;
	mPlayer.init(glm::vec3(10.0f, 10.0f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 100.0f, 100.0f, 30.0f, true, 
				 pSceneManager->getProgramProperties().mResourcePath + "Models/player.obj");
}

void Controler::controlAll(float pDeltaTime)
{
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

	if (mPlayer.isInThirdPersonCamera())
	{
		mSceneManager->getProgramProperties().mViewMatrix = mPlayer.getThirdPersonCamera().getViewMatrix();
		mSceneManager->getProgramProperties().mThirdPersonCam = mPlayer.getThirdPersonCamera();
		mSceneManager->getProgramProperties().mTakeCursor = false;
		mSceneManager->getModelProperties().mProjMatrix = glm::perspective(glm::radians(mPlayer.getThirdPersonCamera().getZoom()),
																		  (float)mSceneManager->getProgramProperties().mWindowWidth /
																		  (float)mSceneManager->getProgramProperties().mWindowHeight, 0.1f, 2000.0f);
	}
	else
	{
		mSceneManager->getProgramProperties().mViewMatrix = mPlayer.getCamera().getViewMatrix();
		mSceneManager->getProgramProperties().mCamera = mPlayer.getCamera();
		mSceneManager->getProgramProperties().mTakeCursor = true;
		mSceneManager->getModelProperties().mProjMatrix = glm::perspective(glm::radians(mPlayer.getCamera().getZoom()),
																		  (float)mSceneManager->getProgramProperties().mWindowWidth /
																		  (float)mSceneManager->getProgramProperties().mWindowHeight, 0.1f, 2000.0f);
	}
	mPlayer.turnOnNoclip(mSceneManager->getProgramProperties().mNoclip);

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
		mPlayer.move(moveSidesPlayer::FORWARD_, pDeltaTime);
	if (mKeyCodes[SDLK_S])
		mPlayer.move(moveSidesPlayer::BACKWARD_, pDeltaTime);
	if (mKeyCodes[SDLK_A])
		mPlayer.move(moveSidesPlayer::LEFT_, pDeltaTime);
	if (mKeyCodes[SDLK_D])
		mPlayer.move(moveSidesPlayer::RIGHT_, pDeltaTime);
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
	float speed = 0.2f;
	if (mKeyCodes[SDLK_UP])
		mSceneManager->getLightProperties().mPosLight.y += speed;
	if (mKeyCodes[SDLK_DOWN])
		mSceneManager->getLightProperties().mPosLight.y -= speed;
	if (mKeyCodes[SDLK_RIGHT])
		mSceneManager->getLightProperties().mPosLight.x -= speed;
	if (mKeyCodes[SDLK_LEFT])
		mSceneManager->getLightProperties().mPosLight.x += speed;
}

Player& Controler::getPlayer() noexcept
{
	return mPlayer;
}

SDL_Event& Controler::getEvents() noexcept
{
	return mEvent;
}
