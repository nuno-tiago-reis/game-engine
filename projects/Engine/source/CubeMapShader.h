#ifndef CUBE_MAP_SHADER_H
#define CUBE_MAP_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

using namespace std;

#define CUBE_MAP_SHADER "Cube Map Shader"

#define CUBE_MAP_VERTEX_SHADER_FILE		"shaders/cube_map_vertex_shader.glsl"
#define CUBE_MAP_FRAGMENT_SHADER_FILE	"shaders/cube_map_fragment_shader.glsl"

#define CUBE_MAP_UNIFORM "CubeMap"

class CubeMapShader : public ShaderProgram {

	protected:

	public:

		/* Constructors & Destructors */
		CubeMapShader(string name);
		~CubeMapShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif