#include "Shader.h"

Shader::Shader(const std::filesystem::path& pPathVertex, const std::filesystem::path& pPathFragment)
{
	init(pPathVertex, pPathFragment);
}

Shader::~Shader()
{
	glDeleteProgram(mShaderID);
}

void Shader::init(const std::filesystem::path& pPathVertex, const std::filesystem::path& pPathFragment)
{
	std::string vertexSource = parsePath(pPathVertex);
	std::string fragmentSource = parsePath(pPathFragment);

	mShaderID = createShaders(vertexSource, fragmentSource);
}

void Shader::bind()
{
	glUseProgram(mShaderID);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::setUniform1i(std::string_view pName, int32_t pValue)
{
	glUniform1i(Shader::getUniformLocation(pName), pValue);
}

void Shader::setUniform1f(std::string_view pName, float pValue)
{
	glUniform1f(Shader::getUniformLocation(pName), pValue);
}

void Shader::setMatrixUniform4fv(std::string_view pName, const glm::mat4& pValue)
{
	glUniformMatrix4fv(getUniformLocation(pName), 1, GL_FALSE, &pValue[0][0]);
}

void Shader::setUniform3fv(std::string_view pName, const glm::vec3& pData)
{
	glUniform3fv(getUniformLocation(pName), 1, &pData[0]);
}

void Shader::setUniform2fv(std::string_view pName, const glm::vec2& pData)
{
	glUniform2fv(getUniformLocation(pName), 1, &pData[0]);
}

std::string Shader::getResourcePath() const noexcept
{
	return RESOURCES_PATH;
}

uint32_t Shader::createShaders(std::string_view pVertexSource, std::string_view pFragmentSource)
{
	uint32_t program = glCreateProgram();
	uint32_t vertexSource = compileShaders(GL_VERTEX_SHADER, std::string(pVertexSource));
	uint32_t fragmentSource = compileShaders(GL_FRAGMENT_SHADER, std::string(pFragmentSource));

	glAttachShader(program, vertexSource);
	glAttachShader(program, fragmentSource);

	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vertexSource);
	glDetachShader(program, fragmentSource);

	glDeleteShader(vertexSource);
	glDeleteShader(fragmentSource);

	return program;
}

uint32_t Shader::compileShaders(GLuint pType, const std::string& pStr)
{
	uint32_t id = glCreateShader(pType);
	const char* src = pStr.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int32_t result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int32_t length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* string = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, string);
		std::cout << std::format("Couldnt compile the shaders: {}\n", pStr);
		glDeleteShader(id);
		return 0;
	}
	return id;
}

std::string Shader::parsePath(const std::filesystem::path& pPath)
{
	std::ifstream ifstream(pPath);
	if (!ifstream.is_open())
	{
		std::cout << "Couldnt open the ifstream!\n";
		return "";
	}
	std::string finalStr, line;
	while (std::getline(ifstream, line))
	{
		finalStr += line + "\n";
	}
	return finalStr;
}

GLuint Shader::getUniformLocation(std::string_view pName)
{
	if (mStorageLocations.contains(std::string(pName)))
		return mStorageLocations[std::string(pName)];

	GLuint location = glGetUniformLocation(mShaderID, std::string(pName).c_str());
	if (location == -1)
		std::cout << std::format("Couldnt find the location of this name: {}\n", pName);
	mStorageLocations.emplace(std::string(pName), location);
	return location;
}
