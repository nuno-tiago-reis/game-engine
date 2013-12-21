#pragma once

#include "GL/glew.h"
#include "GL/glut.h"

#include <fstream>
#include <climits>

#include "Utility.h"
#include "Constants.h"

#include "FrameBuffer.h"

#define VCOORD 0

class PostProcessing
{
	public:
		PostProcessing(const char* name, const char* vertexShaderFile, const char* fragmentShaderFile);
		~PostProcessing(void);
		void createProgram();
		void deleteProgram();
		void draw(FrameBuffer* fb);
		void fakeDraw(FrameBuffer* fb);
		void onCameraReset();
		
		GLint getProgramId();
		GLint getUniformId();
		GLint getAttributeId();
		string _name;

	private:
		/* Shader Program File Paths */
		const char* _vertexShaderFile;
		const char* _fragmentShaderFile;

		/* Shader Program Code */
		const GLchar* _vertexShader;
		const GLchar* _fragmentShader;
		
		/* Shader Vertex & Fragment Program IDs */
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
		
		GLuint program_postproc, uniform_fbo_texture, uniform_fbo_texture2;

		const GLchar* attribute_name;
		const GLchar* uniform_name;
		const GLchar* uniform_name2;
		
		GLuint vbo_fbo_vertices,vao_fbo_vertices;
		
		void initVertices();
		void deleteVertices();

		bool reset;
};

