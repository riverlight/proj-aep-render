#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "AEPComposer.h"


CAEPComposer::CAEPComposer()
{
	_nFrameBuffer = 0;
	_nTextureOut = 0;
	_nWidth = 0;
	_nHeight = 0;

	_nLayerCount = 0;

	_szVertex = "./shader/compose.vs";
	_szFragment = "./shader/compose.fs";
}

CAEPComposer::~CAEPComposer()
{

}

int CAEPComposer::Open(int width, int height)
{
	_nWidth = width;
	_nHeight = height;
	if (Create_FrameBuffer(_nWidth, _nHeight, _nFrameBuffer, _nTextureOut) != 0)
		return -1;

	return 0;
}

int CAEPComposer::Close()
{

	return 0;
}

int CAEPComposer::Add_Layer(CAEPLayer* pLayer)
{
	if (pLayer == NULL)
		return -1;

	_vpLayer.push_back(pLayer);

	CShader* pShader = new CShader(_szVertex, _szFragment);
	if (pShader == NULL)
		return -1;
	pShader->use();
	pShader->setInt("texture1", 0);
	_vpShader.push_back(pShader);

	unsigned int nVAO;
	if (Make_VAO(pShader->getProgramID(), nVAO) != 0)
		return -1;
	_vnVAO.push_back(nVAO);
	
	_nLayerCount++;

	return 0;
}

int CAEPComposer::Render(int nTimeStamp_ms)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _nFrameBuffer);
	glViewport(0, 0, _nWidth, _nHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < _nLayerCount; i++)
	{
		if (nTimeStamp_ms<_vpLayer[i]->_pDesc->_nStartTime_ms || nTimeStamp_ms>_vpLayer[i]->_pDesc->_nEndTime_ms)
			continue;
		unsigned int text0 = _vpLayer[i]->Get_Texture(nTimeStamp_ms);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, text0);
		glViewport(0, 0, _nWidth, _nHeight);
		_vpShader[i]->use();
		glm::mat4 uniPositionMat = glm::mat4(1.0);
		

		float fRotate = _vpLayer[i]->Get_RotateAngle(nTimeStamp_ms);
		vec2 centerPoint_gl = _vpLayer[i]->Get_CenterPoint_gl(nTimeStamp_ms);
		//cout << "rotate : " << fRotate << endl;
		//cout << "progress : " << _vpLayer[i]->Get_Progress(nTimeStamp_ms) << endl;
		//cout << centerPoint_gl.x << "  " << centerPoint_gl.y << endl;
		//cout << _vpLayer[i]->Get_Scale(nTimeStamp_ms).x << "  " << _vpLayer[i]->Get_Scale(nTimeStamp_ms).y << endl;
		uniPositionMat = glm::translate(uniPositionMat, glm::vec3(centerPoint_gl, 0.0f));
		uniPositionMat = glm::rotate(uniPositionMat, glm::radians(fRotate), glm::vec3(0.0, 0.0, 1.0));
		glm::vec3 vec3Scale = glm::vec3(_vpLayer[i]->Get_Scale(nTimeStamp_ms), 0.0);
		uniPositionMat = glm::scale(uniPositionMat, vec3Scale);
		
		_vpShader[i]->setMat4("uniPositionMat", uniPositionMat);
		glBindVertexArray(_vnVAO[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}

int CAEPComposer::Create_FrameBuffer(AEP_IN int width, AEP_IN int height, AEP_OUT unsigned int& nFrameBufer, AEP_OUT unsigned int& nTextureOut)
{
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	unsigned int textureOut;
	glGenTextures(1, &textureOut);
	glBindTexture(GL_TEXTURE_2D, textureOut);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureOut, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << endl;
		return -1;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	nFrameBufer = framebuffer;
	nTextureOut = textureOut;

	return 0;
}

int CAEPComposer::Make_VAO(AEP_IN unsigned int programID, AEP_OUT unsigned int& nVAO)
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0 - 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0 - 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0 - 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0 - 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(glGetAttribLocation(programID, "aPos"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(glGetAttribLocation(programID, "aPos"));
	// color attribute
	glVertexAttribPointer(glGetAttribLocation(programID, "aColor"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(glGetAttribLocation(programID, "aColor"));
	// texture coord attribute
	glVertexAttribPointer(glGetAttribLocation(programID, "aTexCoord"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(glGetAttribLocation(programID, "aTexCoord"));

	nVAO = VAO;

	return 0;
}
