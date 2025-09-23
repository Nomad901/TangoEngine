#include "UI.h"
#include "Program.h"

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

void UI::control(Program& pProgram)
{
	static bool firstTime = true;
	if (firstTime)
	{
		ImGui::SetNextWindowPos({ 1, 1 });
		ImGui::SetNextWindowSize({ 400, 550 });
		ImGui::SetNextWindowCollapsed(true);
		firstTime = false;
	}

	ImGui::Begin("Control", &pProgram.mProgramProperties.mProgIsRunning, ImGuiFocusedFlags_None);
	
	ImGui::Checkbox("Enable wireframe", &pProgram.mProgramProperties.mWireFrameMode);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SliderFloat("surroundNum", &pProgram.surroundNum, -100, 100);
	ImGui::SliderFloat("insideNum", &pProgram.insideNum, -100, 100);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Spacing();

	ImGui::SliderFloat("Ambient R", &pProgram.mMaterialProperties.mAmbient.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Ambient G", &pProgram.mMaterialProperties.mAmbient.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Ambient B", &pProgram.mMaterialProperties.mAmbient.z, 0.0f, 1.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("Diffuse R", &pProgram.mMaterialProperties.mDiffuse.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse G", &pProgram.mMaterialProperties.mDiffuse.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse B", &pProgram.mMaterialProperties.mDiffuse.z, 0.0f, 1.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("Specular R", &pProgram.mMaterialProperties.mSpecular.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular G", &pProgram.mMaterialProperties.mSpecular.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular B", &pProgram.mMaterialProperties.mSpecular.z, 0.0f, 1.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("Scale of shines", &pProgram.mMaterialProperties.mShines, 0.0f, 256.0f);

	ImGui::End();
}

