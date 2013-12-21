#include "GeneratedTexture.h"


GeneratedTexture::GeneratedTexture(GLenum textureFormat, GLfloat alpha, GLfloat beta, GLint octaves)
	:Texture(textureFormat,"noiseTexture"){
		_alpha=alpha;
		_beta=beta;
		_octaves=octaves;
}

GeneratedTexture::~GeneratedTexture(void){}

void GeneratedTexture::generateNoiseTexture(){
	PerlinNoise::init();
	for(int i=0;i<W;i++){
		for(int j=0;j<H;j++){
			for(int k=0;k<D;k++){
				GLfloat fi=(1.0f*i)/W,fj=(1.0f*j)/H, fk=(1.0f*k)/D;
				GLfloat n=PerlinNoise::PerlinNoise3D(fi,fj,fk,_alpha,_beta,_octaves);
				n=n*0.5f+0.5f;
				t[k*W*H+j*W+i]=n;
			}
		}
	}
	glGenTextures(1, &_textureHandler);
	glBindTexture(_textureFormat, _textureHandler);
	glTexParameteri(_textureFormat, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(_textureFormat, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(_textureFormat, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(_textureFormat, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage3D(_textureFormat, 0, GL_R32F, H, W, D, 0, GL_RED, GL_FLOAT, t);
	Utility::checkOpenGLError("problem generating noise texture");
	glBindTexture(_textureFormat, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GeneratedTexture::bind(GLuint textureID) {
	glActiveTexture(textureID);
	glBindTexture(_textureFormat, _textureHandler);
	Utility::checkOpenGLError("ERROR: coiso2.");
}