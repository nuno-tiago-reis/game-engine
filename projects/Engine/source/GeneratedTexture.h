#ifndef GENERATED_TEXTURE_H
#define GENERATED_TEXTURE_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "PerlinNoise.h"

#include "FrameBuffer.h"

#include "Texture.h"

#define H 512
#define W 512
#define D 4
#define S H*W*D

#define NOISE_TEXTURE_UNIFORM "Noise"
#define NOISE_SCALE_UNIFORM "NoiseScale"

class GeneratedTexture : public Texture {

	private:

		/* Noise Generation Parameters */
		GLfloat _noiseAlpha;
		GLfloat _noiseBeta;

		GLint _noiseOctaves;

		GLfloat _noiseScale;

		/* Noise Texture */
		GLfloat _noiseTexture[S];

	public:

		GeneratedTexture(GLenum textureFormat, GLfloat noiseAlpha, GLfloat noiseBeta, GLint noiseOctaves, GLfloat noiseScale);
		~GeneratedTexture();

		/* Noise Generation Method */
		void load();

		/* Bind & Unbind to OpenGL Methods */
		void bind(GLuint textureID);

		/* Getters */
		GLfloat getNoiseAlpha();

		GLfloat getNoiseBeta();

		GLint getNoiseOctaves();

		GLfloat getNoiseScale();

		/* Setters */
		void setNoiseAlpha(GLfloat noiseAlpha);

		void setNoiseBeta(GLfloat noiseBeta);

		void setNoiseOctaves(GLint noiseOctaves);

		void setNoiseScale(GLfloat noiseScale);
};

#endif
