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

	ImGui::SliderFloat("Roughness", &pSceneManager.mModelProperties.mRoughness, 0.0f, 100.0f);

	if (ImGui::Button("Generate"))
	{
		uint32_t size = 512;
		float minHeight = 0.0f;
		float maxHeight = 300.0f;
		reinterpret_cast<MidpointDispTerrain*>(pSceneManager.getModelProperties().mTerrain.get())->createMidpointDispTerrain(size,
			pSceneManager.getModelProperties().mRoughness,
			minHeight,
			maxHeight);
	}

	ImGui::End();
}