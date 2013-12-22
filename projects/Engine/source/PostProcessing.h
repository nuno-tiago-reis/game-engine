#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include "FrameBuffer.h"

#include "ShaderProgram.h"

#include "Utility.h"
#include "Constants.h"

#define POST_PROCESSING_SHADER "Post Processing Shader"

#define POST_PROCESSING_VERTEX_SHADER_FILE		"shaders/post_processing_vertex_shader.glsl"
#define POST_PROCESSING_FRAGMENT_SHADER_FILE	"shaders/post_processing_fragment_shader.glsl"

#define TEXTURE_0_UNIFORM "Texture0"
#define TEXTURE_1_UNIFORM "Texture1"

class PostProcessing : public ShaderProgram {

	private:

		/* Singleton Instance */
		static PostProcessing* instance;

		/* Vertex Array IDs */
		GLuint _vertexArrayObjectID;
		GLuint _vertexBufferObjectID;
	
		/* Constructors & Destructors */
		PostProcessing(string name);
		~PostProcessing();

	public:

		/* Singleton Methods */
		static PostProcessing* getInstance();
		static void destroyInstance();

		/* GPU Creation & Destruction Methods */
		void bindAttributes();
		void bindUniforms();

		/* Scene Methods */
		void draw(FrameBuffer* frameBuffer);

		void reshape(FrameBuffer* frameBuffer);

		/* Getters */
		GLuint getVertexArrayObjectID();
		GLuint getVertexBufferObjectID();

		/* Setters */
		void setVertexArrayObjectID(GLuint vertexArrayObjectID);
		void setVertexBufferObjectID(GLuint vextexBufferObjectID);
};

#endif
