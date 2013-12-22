#include "GL/glew.h"
#include "GL/freeglut.h"
#include "tinyxml2.h"

#include <string>

#include "Object.h"

#define TRANSFORMATION_FILE "models/transformation - Child Nodes.xml"
#define TRANSFORMATION_OUTPUT_FILE "models/transformationOutput.xml"

using namespace tinyxml2;

class XML_Reader {

	private:

		/* Singleton Instance */
		static XML_Reader *instance;

		/* TinyXML2 Document */
		XMLDocument* _transformationDocument;

		XML_Reader();
		~XML_Reader();

	public:

		/* Singleton Methods */
		static XML_Reader* getInstance();
		static void destroyInstance();

		void openTransformationFile(string filename);

		void loadTransformation(Object *graphicObject);
		void saveTransformation(Object *graphicObject);
};


