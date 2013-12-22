#ifndef REAL_WOOD_SHADER_H
#define REAL_WOOD_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

using namespace std;

#define REAL_WOOD_SHADER "Realistic Wood"

#define WOOD_TEXTURE_VERTEX_SHADER_FILE		"shaders/real_wood_vertex_shader.glsl"
#define WOOD_TEXTURE_FRAGMENT_SHADER_FILE	"shaders/real_wood_fragment_shader.glsl"

#define DIFFUSE_TEXTURE_UNIFORM "DiffuseTexture"
#define NORMAL_TEXTURE_UNIFORM "NormalTexture"

class RealWoodShader : public ShaderProgram {

	protected:

	public:
		RealWoodShader(string name);
		~RealWoodShader(void);

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();
};

#endif