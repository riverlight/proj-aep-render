#ifndef AEPFILTER_H
#define AEPFILTER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"

using namespace std;



class CAEPFilter
{
public:
	CAEPFilter(char *szVertex, char *szFragment);
	virtual ~CAEPFilter();

public:
	int Open(int width, int height);
	int Close();
	int Render(unsigned int textureIn);
	int Render(float fProgress, unsigned int textureIn);

	unsigned int Get_framebuffer() { return _framebuffer; }
	unsigned int Get_textureOut() { return _textureOut; }

private:
	unsigned int makeVAO();
	unsigned int createFramebuffer();

private:
	unsigned int _framebuffer;
	unsigned int _textureOut, _textureIn;
	unsigned int _VAO;

	CShader* _pShader;
	char* _szVertex, * _szFragment;

	int _width, _height;
};

#endif // AEPFILTER_H
