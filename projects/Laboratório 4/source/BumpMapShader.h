#ifndef BUMP_MAP_SHADER_H
#define BUMP_MAP_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

using namespace std;

#define BUMPMAP_VERTEX_SHADER_FILE		"shaders/bumpMap_VertexShader.glsl"
#define BUMPMAP_FRAGMENT_SHADER_FILE	"shaders/bumpMap_FragShader.glsl"

class BumpMapShader : public ShaderProgram {

	protected:

	public:

		/* Constructors & Destructors */
		BumpMapShader(const char* name);
		~BumpMapShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif