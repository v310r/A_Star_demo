#pragma once


#define INVALID_SHADER_OBJECT -1

class Shader
{
public:
	Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

	Shader();

	void Bind();

	void Unbind();

	static std::string GetShaderTypeStr(uint32_t shaderType);

	void SetUniformMatrix4fv(const glm::mat4& matrix, const std::string& name);

	void SetUniformVector3f(const glm::vec3& vector, const std::string& name);

	void SetUniformVector4f(const glm::vec4& vector, const std::string& name);


protected:
	uint32_t CompileShader(const std::filesystem::path& shaderPath, uint32_t shaderType);

	int32_t m_Id = 0;
};