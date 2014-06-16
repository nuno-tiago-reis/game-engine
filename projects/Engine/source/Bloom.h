#ifndef BLOOM_H
#define BLOOM_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "PostProcessingEffect.h"

#include "BloomShader.h"

class Bloom : public PostProcessingEffect {

	private:
	
		BloomShader* _bloomShader;

	public:

		/* Constructors & Destructors */
		Bloom(string name);
		~Bloom();

		/* Scene Methods */
		void draw();

		void reshape(GLint width, GLint height);
};

#endif
