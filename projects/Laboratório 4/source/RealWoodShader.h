#ifndef REAL_WOOD_SHADER_H
#define REAL_WOOD_SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ShaderProgram.h"

using namespace std;

#define WOOD_TEXTURE_VERTEX_SHADER_FILE		"shaders/woodVertexShader.glsl"
#define WOOD_TEXTURE_FRAGMENT_SHADER_FILE	"shaders/woodFragmentShader.glsl"

class RealWoodShader : public ShaderProgram {

	protected:

	public:
		RealWoodShader(const char* name);
		~RealWoodShader(void);

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

private:
	const GLchar* ntname;
	const GLchar* nsname;
	GLint ntloc,nsloc;
};

#endif