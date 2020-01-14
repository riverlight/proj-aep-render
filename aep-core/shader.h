#ifndef MY_SHADER_H
#define MY_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace std;

class CShader
{
public:
	// mode==0 is  path mode, or mode==1 is string mode
	CShader(const char* szVertex, const char* szFragment, int mode=0);

	virtual ~CShader();

public:
	unsigned int getProgramID() { return _ID; }

	void use();
	// uniform¹¤¾ßº¯Êý
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	unsigned int _ID;
};


#endif // MY_SHADER_H
