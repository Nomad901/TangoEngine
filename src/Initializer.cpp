#include "Initializer.h"
#include "Program.h"

void Initializer::initAll(Program& pProgram)
{
	initShaders(pProgram);
	initTextures(pProgram);
	initPrimitives(pProgram);
	initMeshes(pProgram);
	initMaterial(pProgram);
	initModels(pProgram);
	initLights(pProgram);
	initCrosshair(pProgram);
	initMousePicker(pProgram);
	initSkybox(pProgram);
	initUBO(pProgram);
}

void Initializer::initShaders(Program& pProgram)
{
	std::string resourcePath = pProgram.mProgramProperties.mShader.getResourcePath();
	// main shader
	pProgram.mProgramProperties.mShader.init(resourcePath + "Shaders/vert.glsl",
											 resourcePath + "Shaders/frag.glsl");

	// was created for implementing source of lights. like white block
	pProgram.mProgramProperties.mShaderSingleColor.init(resourcePath + "Shaders/vert.glsl",
													    resourcePath + "Shaders/shaderSingleColor.glsl");

	// FBO shader
	pProgram.mProgramProperties.mShaderSecondScreen.init(resourcePath + "Shaders/vertFrameBuffer.glsl",
														 resourcePath + "Shaders/fragFrameBuffer.glsl");

	// skybox shader
	pProgram.mProgramProperties.mSkyboxShader.init(resourcePath + "Shaders/vertSkybox.glsl",
												   resourcePath + "Shaders/fragSkybox.glsl");
}

void Initializer::initTextures(Program& pProgram)
{
}

void Initializer::initPrimitives(Program& pProgram)
{
}

void Initializer::initMeshes(Program& pProgram)
{
}

void Initializer::initMaterial(Program& pProgram)
{
}

void Initializer::initModels(Program& pProgram)
{
}

void Initializer::initLights(Program& pProgram)
{
}

void Initializer::initCrosshair(Program& pProgram)
{
}

void Initializer::initMousePicker(Program& pProgram)
{
}

void Initializer::initSkybox(Program& pProgram)
{
}

void Initializer::initUBO(Program& pProgram)
{
}
