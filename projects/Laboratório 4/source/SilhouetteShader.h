#ifndef SILHOUETTE_SHADER_H
#define SILHOUETTE_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

using namespace std;

#define SILHOUETTE_VERTEX_SHADER_FILE	"shaders/silhouette_vertex_shader.glsl"
#define SILHOUETTE_FRAGMENT_SHADER_FILE	"shaders/silhouette_fragment_shader.glsl"

class SilhouetteShader : public ShaderProgram {

	protected:

	public:

		/* Constructors & Destructors */
		SilhouetteShader(const char* name);
		~SilhouetteShader();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Debug Methods */
		void dump();
};

#endif