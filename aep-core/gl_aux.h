#ifndef GL_AUX_H
#define GL_AUX_H

typedef struct TextureInfo_s
{
	unsigned int _textureID;
	int _width, _height;
} ImageTexture;

ImageTexture* createImageTexture(char* szImage);

unsigned int createTexture_from_image(char* szImage);

#endif // GL_AUX_H
