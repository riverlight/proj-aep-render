#ifndef AEPLAYER_H
#define AEPLAYER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "AEPFilter.h"
#include "AEPComposer.h"
#include "gl_aux.h"

using namespace std;
using namespace glm;

class CAEPComposer;

typedef enum EFFECT_TYPE_e
{
	ET_NONE, // 不使用 特效类
	ET_TRANSITION, 
	ET_NUMBER
} EFFECT_TYPE;

typedef struct LayerDesc_s
{
	int _nStartTime_ms, _nEndTime_ms;
	vec2 _vecStartPos[2], _vecEndPos[2]; // 左上角坐标为 (0, 0)，这个和opengl的坐标是不一样的
	float _fStartRotateAngle, _fEndRotateAngle; // 360度表示回到原点
	char* _szImageName;
	EFFECT_TYPE _eEffectType;
	char* _szTransitionTarget;
} LayerDesc;

/* 负责把输入转化成 texture */
class CAEPLayer
{
public:
	CAEPLayer(CAEPComposer *pComposer);
	virtual ~CAEPLayer();

public:
	int Open(LayerDesc* pDesc);
	int Close();

	float Get_RotateAngle(int nTimeStamp_ms);
	vec2 Get_CenterPoint(int nTimeStamp_ms);
	vec2 Get_CenterPoint_gl(int nTimeStamp_ms); // gl的坐标体系，即图像中间坐标为 (0, 0)
	vec2 Get_Scale(int nTimeStamp_ms);
	float Get_Progress(int nTimeStamp_ms);

	int Get_Texture(int nTimeStamp_ms);

public:
	LayerDesc* _pDesc;
	CAEPComposer* _pComposer;

private:
	// input element
	ImageTexture* _pImageTexture;
	ImageTexture* _pTransitionTargetTexture;
	CAEPFilter* _pEffect;


};

#endif // AEPLAYER_H
