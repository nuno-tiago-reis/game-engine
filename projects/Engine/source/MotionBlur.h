#ifndef MOTION_BLUR_H
#define MOTION_BLUR_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

/* Generic Post-processing Effect */
#include "PostProcessingEffect.h"

/* Motion Blur Shader */
#include "MotionBlurShader.h"

class MotionBlur : public PostProcessingEffect {

	private:
	
		MotionBlurShader* motionBlurShader;

	public:

		/* Constructors & Destructors */
		MotionBlur(string name);
		~MotionBlur();

		/* Scene Methods */
		void draw();

		void reshape(GLint width, GLint height);
};

#endif
