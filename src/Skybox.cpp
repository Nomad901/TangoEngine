#include "Skybox.h"

Skybox::Skybox(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, uint32_t pSlot)
{
	init(pTypeSkybox, pPaths, pSlot);
}

void Skybox::init(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, uint32_t pSlot)
{
	Texture2 tmpTexture;
	tmpTexture.initCubeMaps(pPaths);
	if (pTypeSkybox == typeSkybox::CUBE)
	{
		std::shared_ptr<Primitive> cube = std::make_shared<Cube>(tmpTexture, pSlot, true);
		std::weak_ptr<Primitive> weakCube = cube;
		mSkybox.init(weakCube);
	}
	else if (pTypeSkybox == typeSkybox::SPHERE)
	{
		std::shared_ptr<Primitive> sphere = std::make_shared<Sphere>(tmpTexture, pSlot, 3000.0f, 32, 16);
		std::weak_ptr<Primitive> weakSphere = sphere;
		mSkybox.init(weakSphere);
	}
}

Mesh& Skybox::getMesh() noexcept
{
	return mSkybox;
}

Texture2& Skybox::getTexture() noexcept
{
	return mSkybox.getPrimitive().getSingleTex();
}

void Skybox::setMVP(const glm::mat4& pModel, const glm::mat4& pProj, const glm::mat4& pView)
{
	mMVP = pProj * pView * pModel;
}

glm::mat4& Skybox::getMVP() noexcept
{
	return mMVP;
}

void Skybox::render(Shader& pShader)
{
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	pShader.bind();
	pShader.setMatrixUniform4fv("uMVP", mMVP);
	
	getTexture().bind(GL_TEXTURE_CUBE_MAP);
	mSkybox.draw();
	
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}
