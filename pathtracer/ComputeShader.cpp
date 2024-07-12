#include "ComputeShader.h"
#include <glad/glad.h>


ComputeShader::ComputeShader(const char* path)
{

	std::string computeCode;
	std::ifstream shaderFile;
	int success;
	char infoLog[512];

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		computeCode = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "compute shader file reading error" << std::endl;
	}

	const char* shaderCode = computeCode.c_str();

	unsigned int compute;

	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &shaderCode, NULL);
	glCompileShader(compute);


	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "shader compilation failed\n" << infoLog << std::endl;
	};

	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);



	glUseProgram(ID);
	glDeleteShader(ID);

}

void ComputeShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setVec2i(const std::string& name, glm::vec2 value) const
{
	glUniform2i(glGetUniformLocation(ID, name.c_str()), value.x, value.y);

}
void ComputeShader::setVec2(const std::string& name, glm::vec2 value) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);

}

void ComputeShader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void ComputeShader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
