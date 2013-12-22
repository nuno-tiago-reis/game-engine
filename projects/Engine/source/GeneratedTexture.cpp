#include "GeneratedTexture.h"

GeneratedTexture::GeneratedTexture(GLenum textureFormat, GLfloat noiseAlpha, GLfloat noiseBeta, GLint noiseOctaves, GLfloat noiseScale) 
	: Texture(textureFormat) {

	_noiseAlpha = noiseAlpha;
	_noiseBeta = noiseBeta;
	_noiseOctaves = noiseOctaves;

	_noiseScale = noiseScale;
}

GeneratedTexture::~GeneratedTexture() {
}

void GeneratedTexture::load() {

	PerlinNoise* perlinNoise = PerlinNoise::getInstance();

	perlinNoise->init();

	for(int i=0;i<W;i++) {

		for(int j=0;j<H;j++) {

			for(int k=0;k<D;k++) {

				GLfloat fi=(1.0f*i)/W,fj=(1.0f*j)/H, fk=(1.0f*k)/D;
				GLfloat n=perlinNoise->PerlinNoise3D(fi,fj,fk,_noiseAlpha,_noiseBeta,_noiseOctaves);
				n=n*0.5f+0.5f;

				_noiseTexture[k*W*H+j*W+i]=n;
			}
		}
	}

	glGenTextures(1, &_textureHandler);

	glBindTexture(_textureFormat, _textureHandler);

	glTexParameteri(_textureFormat, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(_textureFormat, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(_textureFormat, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(_textureFormat, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage3D(_textureFormat, 0, GL_R32F, H, W, D, 0, GL_RED, GL_FLOAT, _noiseTexture);

	glBindTexture(_textureFormat, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Utility::checkOpenGLError("ERROR: Texture \"" + _fileName + "\" loading failed.");
}

void GeneratedTexture::bind(GLuint textureID) {

	glActiveTexture(textureID);
	glBindTexture(_textureFormat, _textureHandler);

	Utility::checkOpenGLError("ERROR: Texture \"" + _fileName + "\" binding failed.");
}

GLfloat GeneratedTexture::getNoiseAlpha() {

	return _noiseAlpha;
}

GLfloat GeneratedTexture::getNoiseBeta() {

	return _noiseBeta;
}

GLint GeneratedTexture::getNoiseOctaves() {

	return _noiseOctaves;
}

GLfloat GeneratedTexture::getNoiseScale() {

	return _noiseScale;
}

void GeneratedTexture::setNoiseAlpha(GLfloat noiseAlpha) {

	_noiseAlpha = noiseAlpha;
}

void GeneratedTexture::setNoiseBeta(GLfloat noiseBeta) {

	_noiseBeta = noiseBeta;
}

void GeneratedTexture::setNoiseOctaves(GLint noiseOctaves) {

	_noiseOctaves = noiseOctaves;
}

void GeneratedTexture::setNoiseScale(GLfloat noiseScale) {

	_noiseScale = noiseScale;
}