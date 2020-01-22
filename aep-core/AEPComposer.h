#ifndef AEPCOMPOSER_H
#define AEPCOMPOSER_H
#include <vector>

#include "AEPLayer.h"
#include "shader.h"

using namespace std;

#ifndef AEP_IN
#define AEP_IN
#define AEP_OUT
#endif // AEP_IN

class CAEPComposer
{
public:
	CAEPComposer();
	virtual ~CAEPComposer();

public:
	int Open(int width, int height);
	int Close();

	int Add_Layer(CAEPLayer *pLayer);
	int Render(int nTimeStamp_ms);

public:
	unsigned int Get_framebuffer() { return _nFrameBuffer; }
	unsigned int Get_textureOut() { return _nTextureOut; }

private:
	static int Create_FrameBuffer(AEP_IN int width, AEP_IN int height, \
		AEP_OUT unsigned int& nFrameBufer, AEP_OUT unsigned int& nTextureOut);
	static int Make_VAO(AEP_IN unsigned int programID, AEP_OUT unsigned int& nVAO);

private:
	vector<CAEPLayer*> _vpLayer;
	vector<CShader*> _vpShader;
	vector<unsigned int> _vnVAO;
	int _nLayerCount;

	const char* _szVertex, * _szFragment;

private:
	unsigned int _nFrameBuffer;
	unsigned int _nTextureOut;
	int _nWidth, _nHeight;
	
};

#endif // AEPCOMPOSER_H
