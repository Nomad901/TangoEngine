#pragma once
#include "Initializer.h"
#include "Controler.h"
#include "Renderer.h"

class Program
{
public:
	Program(uint32_t pWindowWidth = 1920, uint32_t pWindowHeight = 1080);
	//Program(uint32_t pWindowWidth = 1280, uint32_t pWindowHeight = 720);
	~Program();

	void run();

private:
	static void debugOutput(GLenum source,
							GLenum type,
							GLuint id,
							GLenum severity,
							GLsizei length,
							const GLchar* message,
							const void* userParam);
private:

	SceneManager mSceneManager;
	Renderer mRenderer{ &mSceneManager };
	Initializer mInitializer{ &mSceneManager };
	std::unique_ptr<Controler> mControler;
	friend class Initializer;
	friend class Controler;
};