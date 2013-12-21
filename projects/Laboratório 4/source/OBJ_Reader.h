#include "GL/glew.h"
#include "GL/freeglut.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "GraphicObject.h"

#define LOCATION "models/"

#define X 0
#define Y 1
#define Z 2

typedef struct {
	GLfloat AMBIENT[3];
	GLfloat DIFUSE[3];
	GLfloat SPECULAR[4];

} Material;

class OBJ_Reader{

public:
	OBJ_Reader();
	~OBJ_Reader();

	static void readFile(string path, GraphicObject* graphicObject);
};

//split strings on token
std::vector<int> &split(const std::string &s, char delim, std::vector<int> &elems);
std::vector<int> split(const std::string &s, char delim);