#ifndef AEPLAYER_H
#define AEPLAYER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "gl_aux.h"

using namespace std;
using namespace glm;

typedef struct LayerDesc_s
{
	int _nStartTime_ms, _nEndTime_ms;
	vec2 _vStartPos[4], _vEndPos[4];
	char* _szImageName;
} LayerDesc;

/* 负责把输入转化成 texture */
class CAEPLayer
{
public:
	CAEPLayer();
	virtual ~CAEPLayer();

public:
	int Open(LayerDesc* pDesc);
	int Close();

	int Get_Texture();

public:
	LayerDesc* _pDesc;

private:
	// input element
	ImageTexture* _pImageTexture;
};

#endif // AEPLAYER_H
