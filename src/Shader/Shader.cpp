#include "pch.h"

#include "Shader.h"

// windows should be higher that glad, otherwise warnmings arise
#include <Windows.h>
#include "glad/glad.h"

#include "Utilities/FileUtility.h"


Shader::Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath)
{	
	uint32_t vertexShader = CompileShader(vertexShaderPath, GL_VERTEX_SHADER);
	if (vertexShader == INVALID_SHADER_OBJECT)
	{
		return;
	}

	uint32_t fragmentShader = CompileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
	if (fragmentShader == INVALID_SHADER_OBJECT)
	{
		return;
	}

	m_Id = glCreateProgram();

	glAttachShader(m_Id, vertexShader);
	glAttachShader(m_Id, fragmentShader);

	glLinkProgram(m_Id);
	
	int32_t success;
	glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_Id, 512, nullptr, infoLog);
		LOG_ERROR("Shader Program LINKING failed: {0}", infoLog);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glDeleteProgram(m_Id);

		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader()
{
	LOG_WARN("Created Empty SHADER!");
}

void Shader::Bind()
{
	glUseProgram(m_Id);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

uint32_t Shader::CompileShader(const std::filesystem::path& shaderPath, uint32_t shaderType)
{
	uint32_t shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		LOG_ERROR("glCreateShader({0}) Failed", Shader::GetShaderTypeStr(shaderType));
		glDeleteShader(shader);
		return INVALID_SHADER_OBJECT;
	}

	std::string shaderSource = FileUtility::ReadFile(shaderPath);
	assert(shaderSource != "");

	const char* shaderSourceData = shaderSource.data();
	glShaderSource(shader, 1, &shaderSourceData, nullptr);
	glCompileShader(shader);

	int32_t success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		LOG_ERROR("Shader compilation failed: {0}", infoLog);
		glDeleteShader(shader);
		return INVALID_SHADER_OBJECT;
	}

	return shader;
}

std::string Shader::GetShaderTypeStr(uint32_t shaderType)
{
	switch (shaderType)
	{
	case GL_VERTEX_SHADER:				return "VertexShader";
	case GL_FRAGMENT_SHADER:			return "FragmentShader";
	case GL_GEOMETRY_SHADER:			return "GeometryShader";
	case GL_TESS_CONTROL_SHADER:		return "TesselationControlShader";
	case GL_TESS_EVALUATION_SHADER:		return "TesselationEvaluationShader";
	case GL_COMPUTE_SHADER:				return "ComputeShader";

	default:							return "UnknownShader";
	}
}

void Shader::SetUniformMatrix4fv(const glm::mat4& matrix, const std::string& name)
{
	const int32_t location = glGetUniformLocation(m_Id, name.data());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniformVector3f(const glm::vec3& vector, const std::string& name)
{
	const int32_t location = glGetUniformLocation(m_Id, name.data());
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::SetUniformVector4f(const glm::vec4& vector, const std::string& name)
{
	const int32_t location = glGetUniformLocation(m_Id, name.data());
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}
