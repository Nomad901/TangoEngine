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
	
	ImGui::SliderFloat("Direction light X", &pSceneManager.getLightProperties().mLightDir.x, -500.0f, 500.0f);
	ImGui::SliderFloat("Direction light Y", &pSceneManager.getLightProperties().mLightDir.y, -500.0f, 500.0f);
	ImGui::SliderFloat("Direction light Z", &pSceneManager.getLightProperties().mLightDir.z, -500.0f, 500.0f);
	ImGui::SliderFloat("Light softness", &pSceneManager.getLightProperties().mSoftness, -5.0f, 5.0f);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	static int32_t size = 513;        
	static int32_t octaves = 4;
	static float minHeight = 0.0f;
	static float maxHeight = 200.0f;
	static float amplitude = 50.0f;    
	static float frequency = 0.1f;    
	static float lacunarity = 2.0f;    
	static float persistence = 0.5f;

	ImGui::SliderInt("Size", &size, 256, 1024);
	ImGui::SliderInt("Octaves", &octaves, 1, 15);

	ImGui::SliderFloat("Min height", &minHeight, 0.0f, 1000.0f);
	ImGui::SliderFloat("Max height", &maxHeight, 0.0f, 1000.0f);

	ImGui::SliderFloat("Amplitude", &amplitude, 0.0f, 100.0f);
	ImGui::SliderFloat("Frequency", &frequency, 0.001f, 10.0f);
	ImGui::SliderFloat("Lacunarity", &lacunarity, 0.0f, 10.0f);
	ImGui::SliderFloat("persistence", &persistence, 0.01, 5.0f);

	if (ImGui::Button("Reset"))
	{
		size = 513;
		octaves = 4;
		minHeight = 0.0f;
		maxHeight = 200.0f;
		amplitude = 50.0f;
		frequency = 0.1f;
		lacunarity = 2.0f;
		persistence = 0.5f;
	}

	if (ImGui::Button("Generate"))
	{
		//float roughness = 2.0f;

		// TODO: THE PROBLEM OF TERRAIN IN THE HEIGHT GENERATION!!! ABT MidPointDisp
		reinterpret_cast<FractalNoiseTerrain*>(pSceneManager.getModelProperties().mTerrain.get())->setLight(glm::vec3(1.0f, -1.0f, 0.0f), 0.5f);
		reinterpret_cast<FractalNoiseTerrain*>(pSceneManager.getModelProperties().mTerrain.get())->init(size, minHeight, maxHeight, amplitude,
			frequency, octaves,
			lacunarity, persistence);
		//reinterpret_cast<MidpointDispTerrain*>(mSceneManager->getModelProperties().mTerrain.get())->setPos(glm::vec3(-200.0f, -400.0f, -200.0f));
		pSceneManager.getModelProperties().mTerrain->setHeights(maxHeight - 200.0f, maxHeight - 150.0f, maxHeight - 100.0f, maxHeight - 50.0f);
		pSceneManager.getModelProperties().mTerrain->setOneColor(false);
	}

	ImGui::End();
}