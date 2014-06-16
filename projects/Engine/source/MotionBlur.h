#ifndef MOTION_BLUR_H
#define MOTION_BLUR_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "PostProcessingEffect.h"

#include "MotionBlurShader.h"

class MotionBlur : public PostProcessingEffect {

	private:
	
		MotionBlurShader* _motionBlurShader;

	public:

		/* Constructors & Destructors */
		MotionBlur(string name);
		~MotionBlur();

		/* Scene Methods */
		void draw();

		void reshape(GLint width, GLint height);
};

#endif
