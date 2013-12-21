#include "GL/glew.h"
#include "GL/freeglut.h"
#include "tinyxml2.h"

#include <string>

#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "BoundingVolume.h"

#include "GraphicObject.h"

#define MODEL_FILE "models/model - Child Nodes.xml"
#define MODEL_OUTPUT_FILE "models/modelOutput.xml"

#define TRANSFORMATION_FILE "models/transformation - Child Nodes.xml"
#define TRANSFORMATION_OUTPUT_FILE "models/transformationOutput.xml"

using namespace tinyxml2;

class XML_Editor {

	private:

		/* Singleton Instance */
		static XML_Editor *instance;

		XMLDocument* _modelDocument;
		XMLDocument* _transformationDocument;

		XML_Editor();
		~XML_Editor();

	public:

		/* Singleton Methods */
		static XML_Editor* getInstance();
		static void destroyInstance();

		void openModelFile(string filename);

		void loadModel(GraphicObject* graphicObject);
		void saveModel(GraphicObject *graphicObject);

		void openTransformationFile(string filename);

		void loadTransformation(GraphicObject *graphicObject);
		void saveTransformation(GraphicObject *graphicObject);

		//int shape3DGenerator();

		//int positionGenerator();
};


