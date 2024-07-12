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


	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);


	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "shader compilation failed\n" << infoLog << std::endl;
	};

	glUseProgram(ID);
	glDeleteShader(ID);

}
