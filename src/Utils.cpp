#include "Utils.h"

Utils& Utils::getInstance()
{
	static Utils utils;
	return utils;
}

std::vector<char> Utils::readFromBinaryFile(const std::filesystem::path& pPath,
										    int64_t& pSize)
{
	std::ifstream file(pPath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		throw std::runtime_error("File was not opened!\n");
	
	std::streamsize fileSize = file.tellg();
	pSize = fileSize;
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(fileSize);
	if(!file.read(buffer.data(), fileSize))
		throw std::runtime_error("File was not read!\n");

	return buffer;
}

std::vector<float> Utils::readFromBinaryFile2Float(const std::filesystem::path& pPath, int64_t& pSize)
{
	std::ifstream file(pPath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		throw std::runtime_error("File was not opened!\n");

	std::streamsize fileSize = file.tellg();
	if (fileSize % sizeof(float) != 0)
		throw std::runtime_error("File size is not multiple to float size!\n");

	pSize = fileSize;
	file.seekg(0, std::ios::beg);

	size_t numFloats = fileSize / sizeof(float);
	std::vector<float> buffer(numFloats);
	if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize))
		throw std::runtime_error("Couldnt read data from the file!\n");

	return buffer;
}

std::vector<float> Utils::readFromPNGFile2Float(const std::filesystem::path& pPath, int64_t& pSize)
{
	int32_t width, height, channels;
	uint8_t* imageData = stbi_load(pPath.string().c_str(), &width, &height, &channels, 1);
	if (!imageData)
		throw std::runtime_error("Couldnt load the imageData!\n");
	
	std::vector<float> buffer;
	buffer.reserve(width * height);

	for (int32_t i = 0; i < width * height; ++i)
	{
		float height = imageData[i] / 255.0f;
		buffer.push_back(height);
	}

	return buffer;
}

bool Utils::isPointInsideFrustum(const glm::vec3& pPointPos, const glm::mat4& pMatrix, 
								 float pMultiplier)
{
	glm::vec3 pointPos = pPointPos;
	pointPos.y = -pointPos.y;
	glm::vec4 clipSpace = pMatrix * glm::vec4(pointPos, 1.0f);

	if (clipSpace.w <= 0.0f) 
		return false;

	glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;

	bool isInsideFrustum = (ndc.x >= (-1.0f * pMultiplier) && ndc.x <= (1.0f * pMultiplier) &&
							ndc.y >= (-1.0f * pMultiplier) && ndc.y <= (1.0f * pMultiplier) &&
							ndc.z >= (-1.0f * pMultiplier) && ndc.z <= (1.0f * pMultiplier));

	return isInsideFrustum;
}

float Utils::randomFloatRange(float pStart, float pEnd)
{
	if (pStart == pEnd)
		return pStart;

	if (pEnd < pStart)
	{
		std::cout << "Start is bigger than the end!\n";
		return 0.0f;
	}
	
	float delta = pEnd - pStart;
	float randomValue = randomFloat() * delta + pStart;

	return randomValue;
}

float Utils::randomFloat()
{
	float max = RAND_MAX;
	return ((float)rand() / max);
}

int32_t Utils::calcNextPowerOfTwo(int32_t pX)
{
	int32_t ret = 1;
	if (pX == 1)
		return 2;

	while (ret < pX)
	{
		ret = ret * 2;
	}

	return ret;
}

void Utils::printMatrix4x4(const glm::mat4& pMatrix4x4)
{
	for (uint32_t i = 0; i < 4; ++i) {
		std::cout << std::fixed << std::setprecision(6)
				  << std::setw(12) << pMatrix4x4[i][0] << " "
				  << std::setw(12) << pMatrix4x4[i][1] << " "
				  << std::setw(12) << pMatrix4x4[i][2] << " "
				  << std::setw(12) << pMatrix4x4[i][3] << '\n';
	}
	std::cout << "---\n";
}

void Utils::getGLVersion(int32_t& pMajor, int32_t& pMinor)
{
	glGetIntegerv(GL_MAJOR_VERSION, &pMajor);
	glGetIntegerv(GL_MINOR_VERSION, &pMinor);
}

void Utils::updateDeltaTime(float pDeltaTime)
{
	mDeltaTime = pDeltaTime;
}

float Utils::getDeltaTime()
{
	return mDeltaTime;
}

bool Utils::bufferIsBound(GLenum pTarget, uint32_t pID)
{
	int32_t currentBoundBuffer = 0;
	switch (pTarget)
	{
	case GL_ARRAY_BUFFER: 
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBoundBuffer); 
		break;
	case GL_ELEMENT_ARRAY_BUFFER:
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentBoundBuffer);
		break;
	case GL_UNIFORM_BUFFER:
		glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &currentBoundBuffer);
		break;
	case GL_FRAMEBUFFER:
		break;
	case GL_READ_FRAMEBUFFER:
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentBoundBuffer);
		break;
	case GL_DRAW_FRAMEBUFFER:
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentBoundBuffer);
		break;
	default:
		return false;
	}
	return static_cast<uint32_t>(currentBoundBuffer) == pID;
}

bool Utils::shaderIsBound(uint32_t pID)
{
	int32_t currentShaderBinder = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentShaderBinder);
	return static_cast<uint32_t>(currentShaderBinder) == pID;
}

bool Utils::stencilBufferIsBound()
{
	int32_t attachmentType;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
										  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachmentType);
	return (attachmentType != GL_NONE);
}

std::string Utils::getDirectoryFromFilePath(const std::filesystem::path& pPath)
{
	return pPath.parent_path().string();
}

glm::mat4 Utils::getGlmMatrix4FromAiMat4x4(const aiMatrix4x4& pAiMatrix4x4)
{
	return glm::transpose(glm::mat4(pAiMatrix4x4.a1, pAiMatrix4x4.a2, pAiMatrix4x4.a3, pAiMatrix4x4.a4,  
									pAiMatrix4x4.b1, pAiMatrix4x4.b2, pAiMatrix4x4.b3, pAiMatrix4x4.b4,  
									pAiMatrix4x4.c1, pAiMatrix4x4.c2, pAiMatrix4x4.c3, pAiMatrix4x4.c4,  
									pAiMatrix4x4.d1, pAiMatrix4x4.d2, pAiMatrix4x4.d3, pAiMatrix4x4.d4));
}

aiMatrix4x4 Utils::getAiMatrix4x4FromGlmMatrix4(const glm::mat4& pGlmMatrix4)
{
	return aiMatrix4x4(ai_real(pGlmMatrix4[0][0]), ai_real(pGlmMatrix4[0][1]), ai_real(pGlmMatrix4[0][2]), ai_real(pGlmMatrix4[0][3]),  
					   ai_real(pGlmMatrix4[1][0]), ai_real(pGlmMatrix4[1][1]), ai_real(pGlmMatrix4[1][2]), ai_real(pGlmMatrix4[1][3]),    
					   ai_real(pGlmMatrix4[2][0]), ai_real(pGlmMatrix4[2][1]), ai_real(pGlmMatrix4[2][2]), ai_real(pGlmMatrix4[2][3]),  
					   ai_real(pGlmMatrix4[3][0]), ai_real(pGlmMatrix4[3][1]), ai_real(pGlmMatrix4[3][2]), ai_real(pGlmMatrix4[3][3]));
}
