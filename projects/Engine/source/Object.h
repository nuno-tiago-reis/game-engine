#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include <map>
#include <string>

#include "Matrix.h"

#include "BufferObject.h"

#include "MixedTextureShader.h"
#include "RealWoodShader.h"
#include "SphereMapShader.h"
#include "ShaderProgram.h"
#include "CubeMapShader.h"

#include "Texture.h"
#include "GeneratedTexture.h"
#include "CubeTexture.h"

#define NO_MAPPING 0
#define WOOD_MAPPING 1
#define BUMP_MAPPING 2
#define MIXED_TEXTURE_MAPPING 3
#define SPHERE_MAPPING 4
#define CUBE_MAPPING 5

#define WOOD_TEXTURE "Wood Texture"

#define DIFFUSE_TEXTURE "Diffuse Texture"
#define NORMAL_TEXTURE "Normal Texture"

#define MIXED_TEXTURE_0 "Texture 0"
#define MIXED_TEXTURE_1 "Texture 1"

#define SPHERE_MAP_TEXTURE "Sphere Map Texture"

#define CUBE_MAP_TEXTURE "Cube Map Texture"

using namespace std;

class Object {

	protected:

		/* Graphic Object Identifier */
		string _name;
		string _parentName;

		/* Graphic Object Model Matrix */
		Matrix _modelMatrix;

		/* Graphic Object BufferObject */
		BufferObject* _bufferObject;

		/* Graphic Object Position Vector (Px,Py,Pz,1.0) */
		Vector _position;
		/* Graphic Object Velocity Vector*/
		Vector _velocity;
		/* Graphic Object Scale Factors (Sx,Sy,Sz,1.0) */
		Vector _scale;
		/* Graphic Object Rotation (Rx,Ry,Rz,1.0) */
		Vector _rotation;
		/* Graphic Object Rotation Quaternion */
		Quaternion _rotationQuaternion;

		/* Graphic Object Active Texture */
		GLuint _activeTexture;
		/* Graphic Object Texture Map */
		map<string,Texture*> _textureMap;

	public:

		/* Constructors & Destructors */
		Object(string name);
		~Object();

		/* Scene Methods */
		virtual void draw(GLuint programID);

		virtual void update();
		virtual void update(GLfloat elapsedTime);

		/* Ray Cast Intersection Method */
		GLfloat isIntersecting(Vector origin, Vector direction);

		/* Texture Methods*/
		void activateBumpTexture(string diffuseTextureFileName, string normalTextureFileName);
		void bindBumpTexture(GLuint programID);
		void unbindBumpTexture();

		void activateWoodTexture(GLfloat noiseAlpha, GLfloat noiseBeta, GLint noiseOctaves, GLfloat noiseScale);
		void bindWoodTexture(GLuint programID);
		void unbindWoodTexture();

		void activateMixedTexture(string texture0FileName, string texture1FileName);
		void bindMixedTexture(GLuint programID);
		void unbindMixedTexture();
		
		void activateSphereMapTexture(string sphereMapFileName);
		void bindSphereMapTexture(GLuint programID);
		void unbindSphereMapTexture();

		void activateCubeMapTexture(string positiveXFileName, string negativeXFileName, 
			string positiveYFileName, string negativeYFileName, 
			string positiveZFileName,string negativeZFileName);
		void bindCubeMapTexture(GLuint programID);
		void unbindCubeMapTexture();

		void deactivateTexture();

		/* Getters */
		string getName();
		string getParentName();

		Matrix getModelMatrix();

		BufferObject* getBufferObject();

		Vector getPosition();
		Vector getVelocity();
		Vector getScale();
		Vector getRotation();
		Quaternion getRotationQuaternion();

		/* Setters */
		void setName(string name);
		void setParentName(string parentName);

		void setModelMatrix(Matrix modelMatrix);

		void setBufferObject(BufferObject* bufferObject);

		void setPosition(Vector position);
		void setVelocity(Vector velocity);
		void setScale(Vector scale);
		void setRotation(Vector rotation);
		void setRotationQuaternion(Quaternion rotationQuaternion);

		/* Debug Methods */
		void dump();
};

#endif