
#include "gl_aux.h"
#include "AEPLayer.h"

CAEPLayer::CAEPLayer()
{
	_pDesc = NULL;
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

int CAEPLayer::Get_Texture()
{
	return _pImageTexture->_textureID;
}
