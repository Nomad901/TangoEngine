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

	if (ImGui::Button("Generate"))
	{
		pSceneManager.getModelProperties().mTerrain->setHeights(mHeight0, mHeight1, mHeight2, mHeight3);
	}
	ImGui::End();
}