#ifndef SPHERE_MAP_SHADER_H
#define SPHERE_MAP_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

using namespace std;

#define SPHERE_MAP_SHADER "Sphere Mapping"

#define SPHERE_MAP_VERTEX_SHADER_FILE	"shaders/sphere_map_vertex_shader.glsl"
#define SPHERE_MAP_FRAGMENT_SHADER_FILE	"shaders/sphere_map_fragment_shader.glsl"

#define SPHERE_MAP_UNIFORM "SphereMap"

class SphereMapShader : public ShaderProgram {

	protected:

	public:

		/* Constructors & Destructors */
		SphereMapShader(string name);
		~SphereMapShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif