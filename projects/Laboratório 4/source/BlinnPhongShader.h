#ifndef BLINN_PHONG_SHADER_H
#define BLINN_PHONG_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

using namespace std;

#define BLINN_PHONG_VERTEX_SHADER_FILE		"shaders/blinn_phong_VertexShader.glsl"
#define BLINN_PHONG_FRAGMENT_SHADER_FILE	"shaders/blinn_phong_FragShader.glsl"

class BlinnPhongShader : public ShaderProgram {

	protected:



	public:

		/* Constructors & Destructors */
		BlinnPhongShader(const char* name);
		~BlinnPhongShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif