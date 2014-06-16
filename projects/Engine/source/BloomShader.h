#ifndef BLOOM_SHADER_H
#define BLOOM_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

#include "PostProcessingEffect.h"

using namespace std;

#define BLOOM_SHADER "Bloom Shader"

#define BLOOM_VERTEX_SHADER_FILE	"shaders/bloom_vertex_shader.glsl"
#define BLOOM_FRAGMENT_SHADER_FILE	"shaders/bloom_fragment_shader.glsl"

/* Texture Uniforms */
#define TEXTURE_0_UNIFORM "ColorTexture"
#define TEXTURE_1_UNIFORM "BrightnessTexture"

class BloomShader : public ShaderProgram {

	protected:

	public:

		/* Constructors & Destructors */
		BloomShader(string name);
		~BloomShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif
