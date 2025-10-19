#include "UI.h"
#include "SceneManager.h"

UI::UI(SDL_Window* pWindow, const SDL_GLContext& pContext)
{
	init(pWindow, pContext);
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void UI::init(SDL_Window* pWindow, const SDL_GLContext& pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = nullptr;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForOpenGL(pWindow, pContext);
	ImGui_ImplOpenGL3_Init("#version 440");
}

void UI::control(SceneManager& pSceneManager)
{
	static bool firstTime = true;
	if (firstTime)
	{
		ImGui::SetNextWindowPos({ 1, 1 });
		ImGui::SetNextWindowSize({ 400, 550 });
		ImGui::SetNextWindowCollapsed(true);
		firstTime = false;
	}

	ImGui::Begin("Control", &pSceneManager.mProgramProperties.mProgIsRunning, ImGuiFocusedFlags_None);
	
	ImGui::Checkbox("Wireframe", &pSceneManager.mProgramProperties.mWireFrameMode);
	
	ImGui::Spacing();

	ImGui::Checkbox("Noclip", &pSceneManager.mProgramProperties.mNoclip);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	static float mHeight0{ 130.0f }, mHeight1{ 180.0f }, mHeight2{ 240.0f }, mHeight3{ 300.0f };
	static float mMaxHeight{ 300.0f };

	ImGui::SliderFloat("Height0", &mHeight0, 0.0f, 200.0f);
	ImGui::SliderFloat("Height1", &mHeight1, 50.0f, 300.0f);
	ImGui::SliderFloat("Height2", &mHeight2, 100.0f, 400.0f);
	ImGui::SliderFloat("Height3", &mHeight3, 150.0f, 500.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("MaxHeight", &mMaxHeight, 0.0f, 500.0f);
	ImGui::Text("Default max height: %f", 300.0f);

	ImGui::Spacing();
	ImGui::Spacing();
	
	ImGui::SliderFloat("Direction light X", &pSceneManager.getLightProperties().mLightDir.x, -500.0f, 500.0f);
	ImGui::SliderFloat("Direction light Y", &pSceneManager.getLightProperties().mLightDir.y, -500.0f, 500.0f);
	ImGui::SliderFloat("Direction light Z", &pSceneManager.getLightProperties().mLightDir.z, -500.0f, 500.0f);
	ImGui::SliderFloat("Light softness", &pSceneManager.getLightProperties().mSoftness, -5.0f, 5.0f);

	if (ImGui::Button("Generate"))
	{
		uint32_t size = 512;
		float minHeight = 0.0f;
		float maxHeight = 300.0f;
		float roughness = 1.5f;
		reinterpret_cast<MidpointDispTerrain*>(pSceneManager.getModelProperties().mTerrain.get())->setLight(pSceneManager.getLightProperties().mLightDir, 
																											pSceneManager.getLightProperties().mSoftness);
		reinterpret_cast<MidpointDispTerrain*>(pSceneManager.getModelProperties().mTerrain.get())->createMidpointDispTerrain(size,
			5,
			roughness,
			minHeight,
			maxHeight);
		reinterpret_cast<MidpointDispTerrain*>(pSceneManager.getModelProperties().mTerrain.get())->setPos(glm::vec3(-200.0f, -400.0f, -200.0f));
		pSceneManager.getModelProperties().mTerrain->setHeights(maxHeight - 200.0f, maxHeight - 150.0f, maxHeight - 100.0f, maxHeight - 50.0f);
	}
	ImGui::End();
}