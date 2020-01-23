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


typedef enum FILTERMODE_e
{
	FM_Filter = 0, 
	FM_Effect, 
	FM_Color, 
	FM_Transition, 
	FM_NUMBER
} FILTERMODE;

class CAEPFilter
{
public:
	CAEPFilter(char *szVertex, char *szFragment, FILTERMODE mode= FM_Filter);
	virtual ~CAEPFilter();

public:
	int Open(int width, int height);
	int Close();
	int Render(unsigned int textureIn, unsigned int nLastFrameBuffer=0);
	int Render(float fProgress, unsigned int textureIn, unsigned int nLastFrameBuffer = 0);

	void Set_ColorMap(unsigned int textureColorMap) { _textureColorMap = textureColorMap; }
	void Set_TransitionTarget(unsigned int textureTarget) { _textureTarget = textureTarget;  }
	unsigned int Get_framebuffer() { return _framebuffer; }
	unsigned int Get_textureOut() { return _textureOut; }

private:
	unsigned int makeVAO(unsigned int programID);
	unsigned int createFramebuffer(int width, int height);

private:
	unsigned int _framebuffer;
	unsigned int _textureOut, _textureIn;
	unsigned int _textureColorMap;
	unsigned int _textureTarget;
	unsigned int _VAO;

	CShader* _pShader;
	char* _szVertex, * _szFragment;
	FILTERMODE _mode;

	int _width, _height;
};

#endif // AEPFILTER_H
