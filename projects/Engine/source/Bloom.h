#ifndef BLOOM_H
#define BLOOM_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

/* Generic Post-processing Effect */
#include "PostProcessingEffect.h"

/* HDR Bloom Shader */
#include "BloomShader.h"

class Bloom : public PostProcessingEffect {

	private:
	
		BloomShader* bloomShader;

	public:

		/* Constructors & Destructors */
		Bloom(string name);
		~Bloom();

		/* Scene Methods */
		void draw();

		void reshape(GLint width, GLint height);
};

#endif
