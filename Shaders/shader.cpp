#include "shader.h"
#include <iostream>
std::string Shader::readFile(std::string filename) {
	std::ifstream fin(filename);
	std::string str;
	for (std::string temp; fin && std::getline(fin, temp);) {
		str += temp + "\n";
	}
	fin.close();

	std::cout << str << std::endl;
	return str;
}
GLuint Shader::createShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) {
	std::string vertexSource = readFile(vertexShaderFilename);
	std::string fragmentSource = readFile(fragmentShaderFilename);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *source = (const GLchar *)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);
	glCompileShader(vertexShader);
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	GLint maxLength = 2048;
	if (isCompiled == GL_FALSE) {
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(vertexShader);
		std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
		return -1;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	source = (const GLchar *)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
		return -1;
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
		return -1;
	}
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	return program;
}
Shader::Shader(GLuint ident) {
	this->ident = ident;
}
Shader::Shader() {
	this->ident = 0;
}
void Shader::use() {
	glUseProgram(ident);
}
void Shader::unuse() {
	glUseProgram(0);
}