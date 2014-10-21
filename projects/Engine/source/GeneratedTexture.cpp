#include "GeneratedTexture.h"

GeneratedTexture::GeneratedTexture(string name, GLenum textureFormat, GLfloat noiseAlpha, GLfloat noiseBeta, GLint noiseOctaves, GLfloat noiseScale, string uniform) 
	: Texture(name, textureFormat, uniform) {

	_noiseAlpha = noiseAlpha;
	_noiseBeta = noiseBeta;
	_noiseOctaves = noiseOctaves;

	_noiseScale = noiseScale;
}

GeneratedTexture::~GeneratedTexture() {
}

void GeneratedTexture::loadTexture() {

	/* Initialize the Perlin Noise Generator */
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

	glGenTextures(1, &_handler);

	glBindTexture(_format, _handler);

	glTexParameteri(_format, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(_format, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(_format, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(_format, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage3D(_format, 0, GL_R32F, H, W, D, 0, GL_RED, GL_FLOAT, _noiseTexture);

	glBindTexture(_format, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Utility::checkOpenGLError("ERROR: Texture \"" + _filename + "\" loading failed.");

	/* Destroy Perlin Noise Generator */
	PerlinNoise::destroyInstance();
}

void GeneratedTexture::loadUniforms(GLuint programID, GLuint textureID) {

	glProgramUniform1i(programID, glGetUniformLocation(programID, NOISE_TEXTURE_UNIFORM), textureID);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" NOISE_TEXTURE_UNIFORM "\" error.");

	glProgramUniform1f(programID,glGetUniformLocation(programID,NOISE_SCALE_UNIFORM), _noiseScale);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" NOISE_SCALE_UNIFORM "\" error.");
}

void GeneratedTexture::bind(GLuint textureID) {

	glActiveTexture(textureID);
	glBindTexture(_format, _handler);

	Utility::checkOpenGLError("ERROR: Texture \"" + _filename + "\" binding failed.");
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