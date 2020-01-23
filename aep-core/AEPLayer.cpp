
#include "gl_aux.h"
#include "AEPFilter.h"
#include "AEPLayer.h"

CAEPLayer::CAEPLayer(CAEPComposer* pComposer)
{
	_pDesc = NULL;
	_pComposer = pComposer;
}

CAEPLayer::~CAEPLayer()
{

}

int CAEPLayer::Open(LayerDesc* pDesc)
{
	if (pDesc == NULL)
		return -1;
	_pDesc = new LayerDesc();
	memcpy(_pDesc, pDesc, sizeof(LayerDesc));

	_pImageTexture = createImageTexture(pDesc->_szImageName);
	if (_pImageTexture == NULL)
		return -1;
	if (_pDesc->_eEffectType == ET_TRANSITION)
	{
		_pTransitionTargetTexture = createImageTexture(_pDesc->_szTransitionTarget);
		_pEffect = new CAEPFilter((char*)"./shader/transition_turnpage.vs", (char*)"./shader/transition_turnpage.fs", FM_Transition);
		if (_pEffect->Open(_pImageTexture->_width, _pImageTexture->_height) != 0)
			return -1;
		_pEffect->Set_TransitionTarget(_pTransitionTargetTexture->_textureID);
	}

	return 0;
}

int CAEPLayer::Close()
{
	delete _pImageTexture;
	_pImageTexture = NULL;

	delete _pDesc;
	_pDesc = NULL;

	return 0;
}

float CAEPLayer::Get_Progress(int nTimeStamp_ms)
{
	float fProgress;
	if (_pDesc->_nEndTime_ms == _pDesc->_nStartTime_ms)
		fProgress = 1.0;
	else
		fProgress = float(nTimeStamp_ms - _pDesc->_nStartTime_ms) / float(_pDesc->_nEndTime_ms - _pDesc->_nStartTime_ms);
	if (fProgress < 0.001)
		fProgress = 0;
	return fProgress;
}

float CAEPLayer::Get_RotateAngle(int nTimeStamp_ms)
{
	float fProgress = Get_Progress(nTimeStamp_ms);

	float fAngle;
	fAngle = _pDesc->_fStartRotateAngle + fProgress * (_pDesc->_fEndRotateAngle-_pDesc->_fStartRotateAngle);
	return fAngle;
}

vec2 CAEPLayer::Get_CenterPoint(int nTimeStamp_ms)
{
	float fProgress = Get_Progress(nTimeStamp_ms);

	vec2 startCenter, endCenter, currentCenter;
	startCenter.x = (_pDesc->_vecStartPos[0].x + _pDesc->_vecStartPos[1].x)/2;
	startCenter.y = (_pDesc->_vecStartPos[0].y + _pDesc->_vecStartPos[1].y)/2;
	endCenter.x = (_pDesc->_vecEndPos[0].x + _pDesc->_vecEndPos[1].x) / 2;
	endCenter.y = (_pDesc->_vecEndPos[0].y + _pDesc->_vecEndPos[1].y) / 2;
	currentCenter.x = startCenter.x + fProgress * (endCenter.x - startCenter.x);
	currentCenter.y = startCenter.y + fProgress * (endCenter.y - startCenter.y);
	return currentCenter;
}

vec2 CAEPLayer::Get_CenterPoint_gl(int nTimeStamp_ms)
{
	vec2 centerPoint = Get_CenterPoint(nTimeStamp_ms);
	vec2 centerPoint_gl;
	centerPoint_gl.x = centerPoint.x * 2 - 1;
	centerPoint_gl.y = (1.0 - centerPoint.y) * 2 - 1;
	return centerPoint_gl;
}

vec2 CAEPLayer::Get_Scale(int nTimeStamp_ms)
{
	float fProgress = Get_Progress(nTimeStamp_ms);
	vec2 startScale, endScale, currentScale;
	startScale.x = (_pDesc->_vecStartPos[1].x == _pDesc->_vecStartPos[0].x) ? 0 : (_pDesc->_vecStartPos[1].x - _pDesc->_vecStartPos[0].x);
	startScale.y = (_pDesc->_vecStartPos[1].y == _pDesc->_vecStartPos[0].y) ? 0 : (_pDesc->_vecStartPos[1].y - _pDesc->_vecStartPos[0].y);
	endScale.x = (_pDesc->_vecEndPos[1].x == _pDesc->_vecEndPos[0].x) ? 0 : (_pDesc->_vecEndPos[1].x - _pDesc->_vecEndPos[0].x);
	endScale.y = (_pDesc->_vecEndPos[1].y == _pDesc->_vecEndPos[0].y) ? 0 : (_pDesc->_vecEndPos[1].y - _pDesc->_vecEndPos[0].y);
	currentScale.x = startScale.x + fProgress * (endScale.x - startScale.x);
	currentScale.y = startScale.y + fProgress * (endScale.y - startScale.y);
	return currentScale;
}

int CAEPLayer::Get_Texture(int nTimeStamp_ms)
{
	if (_pDesc->_eEffectType == ET_TRANSITION)
	{
		float fProgress = Get_Progress(nTimeStamp_ms);
		_pEffect->Render(fProgress, _pImageTexture->_textureID, _pComposer->Get_framebuffer());
		return _pEffect->Get_textureOut();
		//return _pTransitionTargetTexture->_textureID;
	} else
		return _pImageTexture->_textureID;
}
