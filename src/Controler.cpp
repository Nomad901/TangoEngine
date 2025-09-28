#include "Controler.h"
#include "Program.h"

Controler::Controler(SceneManager* pSceneManager)
{
	mSceneManager = pSceneManager;
	mPlayer.init(glm::vec3(1.0f, 3.0f, 1.0f), glm::vec3(2.0f, 4.0f, 2.0f), glm::vec3(3.0f, 5.0f, 3.0f), 3.0f);
}

void Controler::controlAll()
{
	mSceneManager->getProgramProperties().mViewMatrix = mPlayer.getCamera().getViewMatrix();
	mSceneManager->getProgramProperties().mCamera = mPlayer.getCamera();
	
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
			mPlayer.getCamera().mouseMovement(glm::vec2(mEvent.motion.xrel, mEvent.motion.yrel));

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
	if (mKeyCodes[SDLK_LSHIFT])
		mPlayer.sprint(true);
	else
		mPlayer.sprint(false);
	if (mKeyCodes[SDLK_W])
		mPlayer.move(moveSidesPlayer::FORWARD);
	if (mKeyCodes[SDLK_S])
		mPlayer.move(moveSidesPlayer::BACKWARD);
	if (mKeyCodes[SDLK_A])
		mPlayer.move(moveSidesPlayer::LEFT);
	if (mKeyCodes[SDLK_D])
		mPlayer.move(moveSidesPlayer::RIGHT);
	if (mKeyCodes[SDLK_SPACE])
		mPlayer.jump(true);
	else
		mPlayer.jump(false);
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
