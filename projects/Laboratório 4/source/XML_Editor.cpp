#include "XML_Editor.h"

XML_Editor* XML_Editor::instance = NULL;

XML_Editor::XML_Editor() {

	_modelDocument = new XMLDocument();
	_transformationDocument = new XMLDocument();
}

XML_Editor::~XML_Editor() {

	delete _modelDocument;
	delete _transformationDocument;
}

XML_Editor* XML_Editor::getInstance() {

	if(instance == NULL)
		instance = new XML_Editor();

	return instance;
}

void XML_Editor::destroyInstance() {

	delete instance;

	instance = NULL;
}

void XML_Editor::openModelFile(string filename) {

	_modelDocument->LoadFile(filename.c_str());

	if(_modelDocument->ErrorID() != XML_NO_ERROR) {
	
		cout << "XML File " << filename.c_str() << " parsing error." << endl << endl;

		cout << _modelDocument->GetErrorStr1() << endl;
		cout << _modelDocument->GetErrorStr2() << endl;
	}
}

void XML_Editor::loadModel(GraphicObject* graphicObject) {

	for(XMLNode* modelXML = _modelDocument->FirstChildElement()->FirstChildElement("model"); modelXML != NULL; modelXML = modelXML->NextSibling()) {

		if(graphicObject->getName().compare(modelXML->ToElement()->Attribute("identifier")) != 0)
			continue;

		int vertexCount = 0;
		int triangleCount = 0;
		
		modelXML->ToElement()->QueryIntAttribute("triangleCount",&triangleCount);

		Vertex *vertices = new Vertex[triangleCount*3];

		for(XMLNode* triangleXML = modelXML->FirstChildElement("triangle"); triangleXML != NULL; triangleXML = triangleXML->NextSibling()) {

			for(XMLNode* vertexXML = triangleXML->FirstChildElement("vertex"); vertexXML != NULL; vertexCount++, vertexXML = vertexXML->NextSibling()) {
				
				vertices[vertexCount].XYZW[0] = vertexXML->ToElement()->FloatAttribute("x");
				vertices[vertexCount].XYZW[1] = vertexXML->ToElement()->FloatAttribute("y");
				vertices[vertexCount].XYZW[2] = vertexXML->ToElement()->FloatAttribute("z");
				vertices[vertexCount].XYZW[3] = vertexXML->ToElement()->FloatAttribute("w");

				vertices[vertexCount].RGBA[0] = vertexXML->ToElement()->FloatAttribute("r");
				vertices[vertexCount].RGBA[1] = vertexXML->ToElement()->FloatAttribute("g");
				vertices[vertexCount].RGBA[2] = vertexXML->ToElement()->FloatAttribute("b");
				vertices[vertexCount].RGBA[3] = vertexXML->ToElement()->FloatAttribute("a");
			}			
		}

		BufferObject* bufferObject = new BufferObject(vertices,vertexCount);
		bufferObject->createBufferObject();

		graphicObject->setBufferObject(bufferObject);

		break;
	}
}

void XML_Editor::saveModel(GraphicObject *graphicObject) {

	XMLDocument* document = new XMLDocument();

	document->LoadFile(MODEL_OUTPUT_FILE);

	XMLElement* model = document->NewElement("model");

	//document->FirstChildElement("geometry")->SetAttribute("count",document->FirstChildElement("geometry")->IntAttribute("count") + 1);

	for(XMLNode* modelIterator = document->FirstChildElement()->FirstChildElement("model"); modelIterator != NULL; modelIterator = modelIterator->NextSibling())
		if(graphicObject->getName().compare(modelIterator->ToElement()->Attribute("identifier")) == 0) {

			document->FirstChildElement("geometry")->DeleteChild(modelIterator); 
			break;
		}

	int triangleCount = graphicObject->getBufferObject()->getVertexCount()/3;

	model->SetAttribute("identifier",graphicObject->getName().c_str());
	model->SetAttribute("triangleCount",triangleCount);

	Vertex* vertices = graphicObject->getBufferObject()->getVertices();

	for(int i=0; i<triangleCount; i++) {

		XMLElement* triangle = document->NewElement("triangle");

			XMLElement* vertex0 = document->NewElement("vertex");
			vertex0->SetAttribute("id",i*3);

			vertex0->SetAttribute("x",vertices[i*3].XYZW[0]);
			vertex0->SetAttribute("y",vertices[i*3].XYZW[1]);
			vertex0->SetAttribute("z",vertices[i*3].XYZW[2]);
			vertex0->SetAttribute("w",vertices[i*3].XYZW[3]);

			vertex0->SetAttribute("r",vertices[i*3].RGBA[0]);
			vertex0->SetAttribute("g",vertices[i*3].RGBA[1]);
			vertex0->SetAttribute("b",vertices[i*3].RGBA[2]);
			vertex0->SetAttribute("a",vertices[i*3].RGBA[3]);

		triangle->InsertEndChild(vertex0);

			XMLElement* vertex1 = document->NewElement("vertex");
			vertex1->SetAttribute("id",i*3+1);

			vertex1->SetAttribute("x",vertices[i*3+1].XYZW[0]);
			vertex1->SetAttribute("y",vertices[i*3+1].XYZW[1]);
			vertex1->SetAttribute("z",vertices[i*3+1].XYZW[2]);
			vertex1->SetAttribute("w",vertices[i*3+1].XYZW[3]);

			vertex1->SetAttribute("r",vertices[i*3+1].RGBA[0]);
			vertex1->SetAttribute("g",vertices[i*3+1].RGBA[1]);
			vertex1->SetAttribute("b",vertices[i*3+1].RGBA[2]);
			vertex1->SetAttribute("a",vertices[i*3+1].RGBA[3]);

		triangle->InsertEndChild(vertex1);

			XMLElement* vertex2 = document->NewElement("vertex");
			vertex2->SetAttribute("id",i*3+2);

			vertex2->SetAttribute("x",vertices[i*3+2].XYZW[0]);
			vertex2->SetAttribute("y",vertices[i*3+2].XYZW[1]);
			vertex2->SetAttribute("z",vertices[i*3+2].XYZW[2]);
			vertex2->SetAttribute("w",vertices[i*3+2].XYZW[3]);

			vertex2->SetAttribute("r",vertices[i*3+2].RGBA[0]);
			vertex2->SetAttribute("g",vertices[i*3+2].RGBA[1]);
			vertex2->SetAttribute("b",vertices[i*3+2].RGBA[2]);
			vertex2->SetAttribute("a",vertices[i*3+2].RGBA[3]);

		triangle->InsertEndChild(vertex2);

		model->InsertEndChild(triangle);
	}

	document->FirstChildElement("geometry")->InsertEndChild(model);
	document->SaveFile(MODEL_OUTPUT_FILE);
}

void XML_Editor::openTransformationFile(string filename) {

	_transformationDocument->LoadFile(filename.c_str());

	if(_transformationDocument->ErrorID() != XML_NO_ERROR) {
	
		cout << "XML File " << filename.c_str() << " parsing error." << endl << endl;

		cout << _transformationDocument->GetErrorStr1() << endl;
		cout << _transformationDocument->GetErrorStr2() << endl;
	}
}

void XML_Editor::loadTransformation(GraphicObject *graphicObject) {

	for(XMLNode* modelXML = _transformationDocument->FirstChildElement()->FirstChildElement("model"); modelXML != NULL; modelXML = modelXML->NextSibling()) {

		if(graphicObject->getName().compare(modelXML->ToElement()->Attribute("identifier")) != 0)
			continue;
			
		graphicObject->setParentName(modelXML->ToElement()->Attribute("parent"));

		XMLNode* boundingVolumeXML = modelXML->FirstChildElement("boundingVolume");

		string boundingVolume(boundingVolumeXML->ToElement()->Attribute("type"));
		
		if(boundingVolume.compare(BOUNDING_BOX) == 0) {

			GLfloat height = boundingVolumeXML->ToElement()->FloatAttribute("height");
			GLfloat width = boundingVolumeXML->ToElement()->FloatAttribute("width");
			GLfloat depth = boundingVolumeXML->ToElement()->FloatAttribute("depth");
		
			BoundingBox* boundingBox = new BoundingBox(height,width,depth);
			graphicObject->setBoundingVolume(boundingBox);
		}
		else if(boundingVolume.compare(BOUNDING_SPHERE) == 0) {

			GLfloat radius = boundingVolumeXML->ToElement()->FloatAttribute("radius");
		
			BoundingSphere* boundingSphere = new BoundingSphere(radius);
			graphicObject->setBoundingVolume(boundingSphere);
		}
		else if(boundingVolume.compare("None") == 0) {
		}
		
		XMLNode* initialTransformationXML = modelXML->FirstChildElement("initial");

		Vector position;

		position[0] = initialTransformationXML->ToElement()->FloatAttribute("px");
		position[1] = initialTransformationXML->ToElement()->FloatAttribute("py");
		position[2] = initialTransformationXML->ToElement()->FloatAttribute("pz");

		graphicObject->setPosition(position);

		Vector scale;
		
		scale[0] = initialTransformationXML->ToElement()->FloatAttribute("sx");
		scale[1] = initialTransformationXML->ToElement()->FloatAttribute("sy");
		scale[2] = initialTransformationXML->ToElement()->FloatAttribute("sz");

		graphicObject->setScale(scale);

		Vector rotation;

		rotation[0] = initialTransformationXML->ToElement()->FloatAttribute("rx");
		rotation[1] = initialTransformationXML->ToElement()->FloatAttribute("ry");
		rotation[2] = initialTransformationXML->ToElement()->FloatAttribute("rz");

		graphicObject->setRotation(rotation);

		break;
	}
}

void XML_Editor::saveTransformation(GraphicObject *graphicObject) {

	XMLDocument* document = new XMLDocument();

	document->LoadFile(TRANSFORMATION_OUTPUT_FILE);

	XMLElement* model = document->NewElement("model");

	//document->FirstChildElement("geometry")->SetAttribute("count",document->FirstChildElement("geometry")->IntAttribute("count") + 1);
	for(XMLNode* modelIterator = document->FirstChildElement()->FirstChildElement("model"); modelIterator != NULL; modelIterator = modelIterator->NextSibling())
		if(graphicObject->getName().compare(modelIterator->ToElement()->Attribute("identifier")) == 0) {

			document->FirstChildElement("geometry")->DeleteChild(modelIterator); 
			break;
		}

	model->SetAttribute("identifier",graphicObject->getName().c_str());
	model->SetAttribute("parent",graphicObject->getParentName().c_str());

		XMLElement* initial = document->NewElement("initial");

		Vector position = graphicObject->getPosition();

		initial->SetAttribute("px",position[0]);
		initial->SetAttribute("py",position[1]);
		initial->SetAttribute("pz",position[2]);

		Vector scale = graphicObject->getScale();

		initial->SetAttribute("sx",scale[0]);
		initial->SetAttribute("sy",scale[1]);
		initial->SetAttribute("sz",scale[2]);

		Vector rotation = graphicObject->getRotation();

		initial->SetAttribute("rx",rotation[0]);
		initial->SetAttribute("ry",rotation[1]);
		initial->SetAttribute("rz",rotation[2]);

	model->InsertFirstChild(initial);

	document->FirstChildElement("geometry")->InsertEndChild(model);
	document->SaveFile(TRANSFORMATION_OUTPUT_FILE);
}