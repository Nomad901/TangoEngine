#include "LightManager.h"

LightManager::LightManager(std::string_view pNameLight, std::unique_ptr<Light>&& pLight)
{
	pushLight(pNameLight, std::move(pLight));
}

auto LightManager::pushLight(std::string_view pNameLight, std::unique_ptr<Light>&& pLight) -> void
{
	mStrgLight.insert_or_assign(std::string(pNameLight), std::move(pLight));
}

auto LightManager::popLight(std::string_view pNameLight) -> void
{
	if (!mStrgLight.contains(std::string(pNameLight)))
	{
		std::cout << "Storage does not contain this!\n";
		return;
	}
	mStrgLight[std::string(pNameLight)] = nullptr;
}

auto LightManager::getLight(std::string_view pNameLight) -> std::expected<std::reference_wrapper<std::unique_ptr<Light>>, std::string_view>
{
	if (!mStrgLight.contains(std::string(pNameLight)))
		return std::unexpected("Storage does not contain this!\n");
	return std::ref(mStrgLight[std::string(pNameLight)]);
}

auto LightManager::getStorageLight() -> std::unordered_map<std::string, std::unique_ptr<Light>>&
{
	return mStrgLight;
}

auto LightManager::getSize() const noexcept -> size_t
{
	return mStrgLight.size();
}

auto LightManager::sendAllToShader(Shader& pShader, Material* pMaterial, bool pIsJustColored) -> void
{
	uint32_t counter = 0;
	pShader.bind();
	for (auto& [key, value] : mStrgLight)
	{
		std::string lightStr = "light[" + std::to_string(counter) + "]";
		pShader.setUniform3fv(lightStr + ".position", value->getPosLight());
		pShader.setUniform3fv(lightStr + ".direction", value->getDirectionLight());
		pShader.setUniform3fv(lightStr + ".color", value->getColor());
		pShader.setUniform3fv(lightStr + ".ambient", value->getAmbient());
		pShader.setUniform3fv(lightStr + ".diffuse", value->getDiffuse());
		pShader.setUniform3fv(lightStr + ".specular", value->getSpecular());

		if (auto spotlight = dynamic_cast<Spotlight*>(value.get()))
		{
			pShader.setUniform1f(lightStr + ".cutOff", spotlight->getCutOff());
		}
		else if (auto flashlight = dynamic_cast<Flashlight*>(value.get()))
		{
			pShader.setUniform1f(lightStr + ".cutOff", flashlight->getCutOff());
			pShader.setUniform1f(lightStr + ".outerCutOff", flashlight->getOuterCutOff());
		}

		pShader.setUniform1f(lightStr + ".intensity", value->getIntensity());
		if (value->getAttenuationSettings().mAttenuationIsUp)
		{
			pShader.setUniform1f(lightStr + ".constant", value->getAttenuationSettings().mConstant);
			pShader.setUniform1f(lightStr + ".linear", value->getAttenuationSettings().mLinear);
			pShader.setUniform1f(lightStr + ".quadratic", value->getAttenuationSettings().mQuadratic);
		}
		pShader.setUniform1i("uNumberOfLights", mStrgLight.size());

		pMaterial->sendToShader(pShader, 0, 1, pIsJustColored);
		counter++;
	}
}