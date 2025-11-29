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
	manageAll(pSceneManager);
}

void UI::beginFrame(std::string_view pName, SceneManager& pSceneManager)
{
	ImGui::Begin(std::string(pName).c_str(), &pSceneManager.getProgramProperties().mProgIsRunning, ImGuiFocusedFlags_None);
}

void UI::endFrame()
{
	ImGui::End();
}

void UI::createWindow(glm::vec2 pPos, glm::vec2 pSize, bool pIsCollapsed)
{
	ImGui::SetNextWindowPos({ pPos.x, pPos.y });
	ImGui::SetNextWindowSize({ pSize.x, pSize.y });
	ImGui::SetNextWindowCollapsed(pIsCollapsed);
}

void UI::setNewWindow(std::string_view pWindowName, glm::vec2 pWindowPos, glm::vec2 pWindowSize)
{
	if (!mWindows.contains(pWindowName))
		mWindows.emplace(pWindowName, true);

	if (mWindows[pWindowName])
	{
		mWindowPos  = pWindowPos;
		mWindowSize = pWindowSize;
		createWindow(mWindowPos, mWindowSize, mWindowIsCollapsed);
		mWindows[pWindowName] = false;
	}
}

void UI::setSpaces(uint32_t pAmountOfSpaces)
{
	for (uint32_t i = 0; i < pAmountOfSpaces; i++)
	{
		ImGui::Spacing();
	}
}

bool UI::setSliderFloat(std::string_view pName, float& pValue, float pMin, float pMax)
{
	if (ImGui::SliderFloat(std::string(pName).c_str(), &pValue, pMin, pMax))
		return true;
	return false;
}

bool UI::setSliderInt(std::string_view pName, int32_t& pValue, int32_t pMin, int32_t pMax)
{
	if (ImGui::SliderInt(std::string(pName).c_str(), &pValue, pMin, pMax))
		return true;
	return false;
}

bool UI::setSlidersVec2(glm::vec2& pVec2, const std::unordered_map<std::string_view, std::pair<float, float>>& pParam)
{
	if (pParam.empty())
		return false;

	if (pParam.size() == 1)
	{
		float vecForSliderFloat2[2] = { pVec2.x, pVec2.y };
		auto it = pParam.begin();
		if (ImGui::SliderFloat2(std::string(it->first).c_str(), vecForSliderFloat2, it->second.first, it->second.second))
		{
			pVec2[0] = vecForSliderFloat2[0];
			pVec2[1] = vecForSliderFloat2[1];
			return true;
		}
	}
	else
	{
		uint32_t counter = 0;
		for (auto& [name, minMax] : pParam)
		{
			ImGui::SliderFloat(std::string(name).c_str(), &pVec2[counter], minMax.first, minMax.second);
			counter++;
		}
		return true;
	}
	return false;
}

bool UI::setSlidersVec3(glm::vec3& pVec3, const std::unordered_map<std::string_view, std::pair<float, float>>& pParam)
{
	if (pParam.empty())
		return false;

	if (pParam.size() == 1)
	{
		float vecForSliderFloat3[3] = { pVec3.x, pVec3.y, pVec3.z };
		auto it = pParam.begin();
		if (ImGui::SliderFloat3(std::string(it->first).c_str(), vecForSliderFloat3, it->second.first, it->second.second))
		{
			pVec3[0] = vecForSliderFloat3[0];
			pVec3[1] = vecForSliderFloat3[1];
			pVec3[2] = vecForSliderFloat3[2];
			return true;
		}
	}
	else
	{
		uint32_t counter = 0;
		for (auto& [name, minMax] : pParam)
		{
			ImGui::SliderFloat(std::string(name).c_str(), &pVec3[counter], minMax.first, minMax.second);
			counter++;
		}
		return true;
	}
	return false;
}

bool UI::setSlidersVec4(glm::vec4& pVec4, const std::unordered_map<std::string_view, std::pair<float, float>>& pParam)
{
	if (pParam.empty())
		return false;

	if (pParam.size() == 1)
	{
		float vecForSliderFloat4[4] = { pVec4.x, pVec4.y, pVec4.z, pVec4.w };
		auto it = pParam.begin();
		if (ImGui::SliderFloat4(std::string(it->first).c_str(), vecForSliderFloat4, it->second.first, it->second.second))
		{
			pVec4[0] = vecForSliderFloat4[0];
			pVec4[1] = vecForSliderFloat4[1];
			pVec4[2] = vecForSliderFloat4[2];
			pVec4[3] = vecForSliderFloat4[3];
			return true;
		}
	}
	else
	{
		uint32_t counter = 0;
		for (auto& [name, minMax] : pParam)
		{
			ImGui::SliderFloat(std::string(name).c_str(), &pVec4[counter], minMax.first, minMax.second);
			counter++;
		}
		return true;
	}
	return false;
}

bool UI::setColorVec3(std::string_view pName, glm::vec3& pVecColor)
{
	float colorFont[3] = { pVecColor.x,
						   pVecColor.y,
						   pVecColor.z };
	if (ImGui::ColorEdit3(std::string(pName).c_str(), colorFont))
	{
		pVecColor.x = colorFont[0];
		pVecColor.y = colorFont[1];
		pVecColor.z = colorFont[2];
		return true;
	}
	return false;
}

bool UI::setColorVec4(std::string_view pName, glm::vec4& pVecColor)
{
	float colorFont[4] = { pVecColor.x,
					       pVecColor.y,
					       pVecColor.z,
						   pVecColor.w };
	if (ImGui::ColorEdit3(std::string(pName).c_str(), colorFont))
	{
		pVecColor.x = colorFont[0];
		pVecColor.y = colorFont[1];
		pVecColor.z = colorFont[2];
		pVecColor.w = colorFont[3];
		return true;
	}
	return false;
}

void UI::manageTerrain(SceneManager& pSceneManager)
{
	setNewWindow("Control", mWindowPos, mWindowSize);

	beginFrame("Control", pSceneManager);

	// -------- CONTROLING --------
	ImGui::Checkbox("Wireframe", &pSceneManager.mProgramProperties.mWireFrameMode);
	setSpaces(1);
	ImGui::Checkbox("Noclip", &pSceneManager.mProgramProperties.mNoclip);
	setSpaces(3);

	static int32_t size = 529;
	//static int32_t octaves = 4;
	static int32_t octaves = 8;
	static float minHeight = 0.0f;
	//static float maxHeight = 200.0f;
	static float maxHeight = 400.0f;
	static float amplitude = 50.0f;
	static float frequency = 0.1f;
	static float lacunarity = 2.0f;
	static float persistence = 0.5f;

	setSliderInt("Octaves", octaves, 1, 15);

	setSliderFloat("Min height", minHeight, 0.0f, 1000.0f);
	setSliderFloat("Max height", maxHeight, 0.0f, 1000.0f);

	setSliderFloat("Amplitude", amplitude, 0.0f, 100.0f);
	setSliderFloat("Frequency", frequency, 0.001f, 10.0f);
	setSliderFloat("Lacunarity", lacunarity, 0.0f, 10.0f);
	setSliderFloat("persistence", persistence, 0.01, 5.0f);

	if (ImGui::Button("Reset"))
	{
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
		reinterpret_cast<FractalNoiseTerrain*>(pSceneManager.getModelProperties().mTerrain.get())->setLight(pSceneManager.getLightProperties().mLightDir,
			pSceneManager.getLightProperties().mSoftness);
		reinterpret_cast<FractalNoiseTerrain*>(pSceneManager.getModelProperties().mTerrain.get())->init(size, minHeight, maxHeight, amplitude,
			frequency, octaves,
			lacunarity, persistence);

		pSceneManager.getModelProperties().mTerrain->setHeights(maxHeight - 200.0f, maxHeight - 150.0f, maxHeight - 100.0f, maxHeight - 50.0f);
		pSceneManager.getModelProperties().mTerrain->setOneColor(false);
	}
	// ----------------------------

	endFrame();
}

void UI::manageFonts(SceneManager& pSceneManager)
{
	setNewWindow("Font", glm::vec2(400.0f, 1.0f), glm::vec2(400.0f, 200.0f));
	
	beginFrame("Font", pSceneManager);

	// -------- CONTROLING --------
	static bool showFps = true;
	if (ImGui::Checkbox("Show fps", &showFps))
		pSceneManager.getProgramProperties().mFontSystem.makeShown(showFps);

	setSliderFloat("Scale", pSceneManager.getProgramProperties().mScaleFont, 0.1f, 10.0f);
	setSlidersVec2(pSceneManager.getProgramProperties().mPosFont, { 
		{"Position X", std::make_pair(0.0f, static_cast<float>(pSceneManager.getProgramProperties().mWindowWidth))},
		{"Position Y", std::make_pair(0.0f, static_cast<float>(pSceneManager.getProgramProperties().mWindowHeight))} 
	});

	setColorVec3("Color", pSceneManager.getProgramProperties().mColorFont);
	// ----------------------------

	endFrame();
}

void UI::manageLight(SceneManager& pSceneManager)
{
	setNewWindow("Light", glm::vec2(800.0f, 1.0f), glm::vec2(400.0f, 200.0f));

	beginFrame("Light", pSceneManager);

	static glm::vec3 lightColor = glm::vec3(1.0f);
	static glm::vec3 lightPos	= glm::vec3(1.0f);

	if (setSlidersVec3(lightPos,
		{
			{"Pos x", std::make_pair(-1000.0f, 1000.0f)},
			{"Pos y", std::make_pair(-1000.0f, 1000.0f)},
			{"Pos z", std::make_pair(-1000.0f, 1000.0f)}
		}))
	{
		pSceneManager.getLightProperties().mSun.setPosLight(lightPos);
	}
	if (setColorVec3("Color Sun", lightColor))
		pSceneManager.getLightProperties().mSun.setColor(lightColor);

	// -------- CONTROLING --------
	//setSlidersVec3(pSceneManager.getLightProperties().mLightDir, { {"Direction", std::make_pair(-500.0f, 500.0f)} });
	//setSliderFloat("Light softness", pSceneManager.getLightProperties().mSoftness, 0.0f, 50.0f);
	//setSpaces(2);
	//setSlidersVec3(pSceneManager.getLightProperties().mPosLight, { {"Position", std::make_pair(-500.0f, 500.0f)} });
	//setSpaces(2);
	//setSliderFloat("Light Radius", pSceneManager.getLightProperties().mRadius, 1.0f, 500.0f);
	//setSpaces(1);
	//setSliderFloat("Ambient intensity", pSceneManager.getLightProperties().mAmbientIntensity, -50.0f, 50.0f);
	//setSliderFloat("Diffuse intensiyt", pSceneManager.getLightProperties().mDiffuseIntensity, -50.0f, 50.0f);
	//setSpaces(3);
	//setSlidersVec3(pSceneManager.getLightProperties().lightPositions[0], { { "Position light 1", std::make_pair(0.0f, 200.0f) } });
	//setSlidersVec3(pSceneManager.getLightProperties().lightPositions[1], { { "Position light 2", std::make_pair(0.0f, 200.0f) } });
	//setSlidersVec3(pSceneManager.getLightProperties().lightPositions[2], { { "Position light 3", std::make_pair(0.0f, 200.0f) } });
	//setSlidersVec3(pSceneManager.getLightProperties().lightPositions[3], { { "Position light 4", std::make_pair(0.0f, 200.0f) } });

	// ----------------------------

	endFrame();
}

void UI::manageSystem(SceneManager& pSceneManager)
{
	setNewWindow("System", glm::vec2(1.0f, 20.0f), glm::vec2(400.0f, 200.0f));

	beginFrame("System", pSceneManager);

	setSliderInt("Current texture", pSceneManager.mProgramProperties.currentTexture, 0, 4);

	setSpaces(2);

	setSliderInt("Current number of quad to display", pSceneManager.mModelProperties.displayedQuad, 0, 2);

	endFrame();
}

void UI::manageCharacter(SceneManager& pSceneManager)
{
	setNewWindow("Character", glm::vec2(400.0f, 20.0f), glm::vec2(400.0f, 200.0f));

	beginFrame("Character", pSceneManager);
	
	setSlidersVec3(pSceneManager.getModelProperties().mCharPos, 
		{ 
			{"Pos x", std::make_pair(-1000.0f, 1000.0f)},
			{"Pos y", std::make_pair(-1000.0f, 1000.0f)},
			{"Pos z", std::make_pair(-1000.0f, 1000.0f)}
		});
	setSlidersVec3(pSceneManager.getModelProperties().mCharRotation,
		{
			{"Rotation x", std::make_pair(-360.0f, 360.0f)},
			{"Rotation y", std::make_pair(-360.0f, 360.0f)},
			{"Rotation z", std::make_pair(-360.0f, 360.0f)}
		});
	setSlidersVec3(pSceneManager.getModelProperties().mCharScale,
		{
			{"Scale x", std::make_pair(-100.0f, 100.0f)},
			{"Scale y", std::make_pair(-100.0f, 100.0f)},
			{"Scale z", std::make_pair(-100.0f, 100.0f)}
		});

	endFrame();
}

void UI::manageWater(SceneManager& pSceneManager)
{
	setNewWindow("Water", glm::vec2(800.0f, 20.0f), glm::vec2(400.0f, 200.0f));

	beginFrame("Water", pSceneManager);

	setSlidersVec3(pSceneManager.getProgramProperties().mWaterPos,
		{
			{"Pos x", std::make_pair(-10000.0f, 10000.0f)},
			{"Pos y", std::make_pair(-10000.0f, 10000.0f)},
			{"Pos z", std::make_pair(-10000.0f, 10000.0f)}
		});
	setSlidersVec3(pSceneManager.getProgramProperties().mWaterScale,
		{
			{"Scale x", std::make_pair(0.0f, 10000.0f)},
			{"Scale y", std::make_pair(0.0f, 10000.0f)},
			{"Scale z", std::make_pair(0.0f, 10000.0f)}
		});
	setColorVec4("Water color", pSceneManager.getProgramProperties().mWaterColor);

	endFrame();
}

void UI::manageAll(SceneManager& pSceneManager)
{
	manageTerrain(pSceneManager);
	manageFonts(pSceneManager);
	manageLight(pSceneManager);
	manageSystem(pSceneManager);
	manageCharacter(pSceneManager);
	manageWater(pSceneManager);
}
