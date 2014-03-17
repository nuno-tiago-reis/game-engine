#include "GL/glew.h"
#include "GL/freeglut.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <string>
#include <vector>

#define LOCATION "models/"

typedef struct {

	float r;
	float g;
	float b;

	float Kd;
	float Ks;

	float shine;

	float transmittance;

	float indexOfRefraction;

} MaterialProperty;

typedef struct {

	GLfloat x;
	GLfloat y;
	GLfloat z;

} Coordinate3D;

typedef struct {

	GLfloat centerX;
	GLfloat centerY;
	GLfloat centerZ;

	float radius;

} Sphere;

using namespace std;

class NFF_Reader {

	private:
		static NFF_Reader *instance;

		NFF_Reader();
		~NFF_Reader();

	public:
		
		static NFF_Reader* getInstance();
		static void destroyInstance();

		void loadModel(string modelFileName);
};

