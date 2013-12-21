#pragma once
#include "PerlinNoise.h"
#include "texture.h"
#include "FrameBuffer.h"

#define H 512
#define W 512
#define D 4
#define S H*W*D
class GeneratedTexture :
	public Texture
{
public:
	GeneratedTexture(GLenum textureFormat, GLfloat alpha, GLfloat beta, GLint octaves);
	~GeneratedTexture(void);
	void generateNoiseTexture();
	using Texture::bind;
	void bind(GLuint textureID);
private:
	GLfloat t[S];
	GLfloat _alpha, _beta;
	GLint _octaves;
};

