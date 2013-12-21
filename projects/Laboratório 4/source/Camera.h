#ifndef CAMERA_H
#define CAMERA_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include "Matrix.h"

#define ORTHOGONAL_NAME "ORTHOGONAL"
#define PERSPECTIVE_NAME "PERSPECTIVE"

#define ORTHOGONAL 0
#define PERSPECTIVE 1

#define ORTHO_LEFT -10.0f
#define ORTHO_RIGHT 10.0f
#define ORTHO_TOP 10.0f
#define ORTHO_BOTTOM -10.0f
#define ORTHO_FAR 30.0f
#define ORTHO_NEAR 1.0f

#define PERS_ANGLE 30.0f
#define PERS_NEAR 1.0f
#define PERS_FAR 10.0f

#define CAMERA_RADIUS 7.5f

/*
 *	Class for the camera entity.
 *  Although only one is used right now, being seperate makes it possible to manage multiple ones.
 */

class Camera {

	private:

		/* Camera Name */
		string _name;

		/* Scene View and Projection Matrix */
		Matrix _viewMatrix;
		Matrix _projectionMatrix;

		/* Scene Projection Mode (Perspective or Orthogonal) */
		GLint _projectionMode;

		/* Shader Buffer Index for View & Projection Matrix */
		GLuint _uniformBufferIndex;

		/* Viewport Width & Height */
		GLint _width;
		GLint _height;

		/* Camera Zoom */
		GLfloat _zoom;

		/* Camera Spherical Coordinates */
		GLfloat _longitude;
		GLfloat _latitude;

		/* Camera Position */
		Vector _position;

		/* Look At Vectors */
		Vector _target;
		Vector _eye;
		Vector _up;

	public:

		/* Constructors & Destructors */
		Camera(string name);
		~Camera();

		/* Camera Methods */
		void update(GLint zoom, GLint longitude, GLint latitude, GLfloat elapsedTime);

		void reshape(GLint width, GLint height);

		/* View & Projection Methods */
		void loadUniforms();

		void loadView();

		void loadOrthogonalProjection();
		void loadOrthogonalProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ, GLfloat farZ);

		void loadPerspectiveProjection();
		void loadPerspectiveProjection(GLfloat angle, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ);

		/* Getters */
		string getName();
		
		Matrix getViewMatrix();
		Matrix getProjectionMatrix();

		GLint getProjectionMode();
		GLuint getUniformBufferIndex();

		GLint getWidth();
		GLint getHeight();

		GLfloat getZoom();
		GLfloat getLongitude();
		GLfloat getLatitude();

		Vector getPosition();

		Vector getTarget();
		Vector getEye();
		Vector getUp();

		/* Setters */
		void setName(const char* name);
		
		void setViewMatrix(Matrix viewMatrix);
		void setProjectionMatrix(Matrix projectionMatrix);

		void setProjectionMode(GLint projectionMode);
		void setUniformBufferIndex(GLuint uniformBufferIndex);

		void setWidth(GLint width);
		void setHeight(GLint height);

		void setZoom(GLfloat zoom);
		void setLongitude(GLfloat longitude);
		void setLatitude(GLfloat latitude);

		void setPosition(Vector position);

		void setTarget(Vector target);
		void setEye(Vector eye);
		void setUp(Vector up);

		/* Debug Methods */
		void dump();
};

#endif