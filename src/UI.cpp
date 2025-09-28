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
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SliderFloat("Pos museum X", &pSceneManager.posMuseum.x, -3000.0f, 3000.0f);
	ImGui::SliderFloat("Pos museum Y", &pSceneManager.posMuseum.y, -3000.0f, 3000.0f);
	ImGui::SliderFloat("Pos museum Z", &pSceneManager.posMuseum.z, -3000.0f, 3000.0f);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SliderFloat("Size museum X", &pSceneManager.sizeMuseum.x, -150.0f, 150.0f);
	ImGui::SliderFloat("Size museum Y", &pSceneManager.sizeMuseum.y, -150.0f, 150.0f);
	ImGui::SliderFloat("Size museum Z", &pSceneManager.sizeMuseum.z, -150.0f, 150.0f);

	ImGui::End();
}

