#ifndef MIXED_TEXTURE_SHADER_H
#define MIXED_TEXTURE_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

/* Generic Shader */
#include "ShaderProgram.h"

using namespace std;

/* Mixed Texture Shader definitions */
#define MIXED_TEXTURE_SHADER "Mixed Texture Shader"

#define MIXED_TEXTURE_VERTEX_SHADER_FILE	"shaders/mixed_texture_vertex_shader.glsl"
#define MIXED_TEXTURE_FRAGMENT_SHADER_FILE	"shaders/mixed_texture_fragment_shader.glsl"

#define MIXED_TEXTURE_0_UNIFORM "Texture0"
#define MIXED_TEXTURE_1_UNIFORM "Texture1"

class MixedTextureShader : public ShaderProgram {

	protected:

	public:

		/* Constructors & Destructors */
		MixedTextureShader(string name);
		~MixedTextureShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif