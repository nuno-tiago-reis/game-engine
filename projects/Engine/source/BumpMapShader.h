#ifndef BUMP_MAP_SHADER_H
#define BUMP_MAP_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

using namespace std;

#define BUMPMAP_SHADER "Bump Mapping"

#define BUMPMAP_VERTEX_SHADER_FILE		"shaders/bump_map_vertex_shader.glsl"
#define BUMPMAP_FRAGMENT_SHADER_FILE	"shaders/bump_map_fragment_shader.glsl"

#define DIFFUSE_TEXTURE_UNIFORM "DiffuseTexture"
#define NORMAL_TEXTURE_UNIFORM "NormalTexture"

class BumpMapShader : public ShaderProgram {

	protected:

	public:

		/* Constructors & Destructors */
		BumpMapShader(string name);
		~BumpMapShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif