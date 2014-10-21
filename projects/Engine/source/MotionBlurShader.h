#ifndef MOTION_BLUR_SHADER_H
#define MOTION_BLUR_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

#include "PostProcessingEffect.h"

using namespace std;

#define MOTION_BLUR_SHADER "Motion Blur Shader"

#define MOTION_BLUR_VERTEX_SHADER_FILE		"shaders/motion_blur_vertex_shader.glsl"
#define MOTION_BLUR_FRAGMENT_SHADER_FILE	"shaders/motion_blur_fragment_shader.glsl"

/* Texture Uniforms */
#define FRAME_0_TEXTURE_UNIFORM "Texture0"
#define FRAME_1_TEXTURE_UNIFORM "Texture1"

class MotionBlurShader : public ShaderProgram {

	protected:

	public:

		/* Constructors & Destructors */
		MotionBlurShader(string name);
		~MotionBlurShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif
