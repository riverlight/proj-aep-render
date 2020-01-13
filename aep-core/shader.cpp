#include "shader.h"

CShader::CShader(const char* szVertex, const char* szFragment, int mode)
{
	const char* vShaderCode;
	const char* fShaderCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::string vertexCode;
	std::string fragmentCode;

	if (mode == 1)
	{
		vShaderCode = szVertex;
		fShaderCode = szFragment;
	}
	else
	{
		// 保证ifstream对象可以抛出异常：
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// 打开文件
			vShaderFile.open(szVertex);
			fShaderFile.open(szFragment);
			std::stringstream vShaderStream, fShaderStream;
			// 读取文件的缓冲内容到数据流中
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// 关闭文件处理器
			vShaderFile.close();
			fShaderFile.close();
			// 转换数据流到string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		vShaderCode = vertexCode.c_str();
		fShaderCode = fragmentCode.c_str();
	}

	// 2. 编译着色器
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// 顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// 打印编译错误（如果有的话）
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// 片元着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// 打印编译错误（如果有的话）
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// 着色器程序
	_ID = glCreateProgram();
	glAttachShader(_ID, vertex);
	glAttachShader(_ID, fragment);
	glLinkProgram(_ID);
	// 打印连接错误（如果有的话）
	glGetProgramiv(_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

CShader::~CShader()
{

}

void CShader::use()
{
	glUseProgram(_ID);
}

void CShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(_ID, name.c_str()), (int)value);
}
void CShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(_ID, name.c_str()), value);
}
void CShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(_ID, name.c_str()), value);
}

void CShader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
