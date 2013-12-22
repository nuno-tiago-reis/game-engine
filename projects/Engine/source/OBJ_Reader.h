#include "GL/glew.h"
#include "GL/freeglut.h"

#include <fstream>
#include <sstream>

#include <string>
#include <vector>

#include "Object.h"

#define LOCATION "models/"

#define X 0
#define Y 1
#define Z 2

typedef struct {

	GLfloat ambient[3];
	GLfloat diffuse[3];
	GLfloat specular[3];

	GLfloat specularConstant;

} Material;

class OBJ_Reader {

	private:
		/* Singleton Instance */
		static OBJ_Reader *instance;

		OBJ_Reader();
		~OBJ_Reader();

	public:

		/* Singleton Methods */
		static OBJ_Reader* getInstance();
		static void destroyInstance();

		void loadModel(string path, Object* graphicObject);
};