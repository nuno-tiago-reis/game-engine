#include "OBJ_Reader.h"

OBJ_Reader* OBJ_Reader::instance = NULL;

OBJ_Reader::OBJ_Reader() {
}

OBJ_Reader::~OBJ_Reader() {
}

OBJ_Reader* OBJ_Reader::getInstance() {

	if(instance == NULL)
		instance = new OBJ_Reader();

	return instance;
}

void OBJ_Reader::destroyInstance() {

	delete instance;

	instance = NULL;
}

std::vector<int> &split(const std::string &s, char delim, std::vector<int> &elems);
std::vector<int> split(const std::string &s, char delim);

void OBJ_Reader::loadModel(string modelFilename, string materialFilename, Object* graphicObject){
	
	string line;
	string activeMaterial;

	int facesN = 0, verticesN = 0, uvcoordsN = 0, normalsN = 0, vertexCount = 0;

	bool materials = false;

	cout << modelFilename << " - " << materialFilename << endl;

	//first read all materials
	map<string,Material> materialsMap;

	std::ifstream materialFile(LOCATION + materialFilename);

	while(getline(materialFile, line)) {

		materials = true;

		istringstream iss(line);

		string start;
		iss >> start;
		
		if(start == "Kd") {

			float x,y,z;

			iss >> x >> y >> z;

			materialsMap[activeMaterial].diffuse[0] = x;
			materialsMap[activeMaterial].diffuse[1] = y;
			materialsMap[activeMaterial].diffuse[2] = z;
		}
		else if(start == "Ka") {

			float x,y,z;

			iss >> x >> y >> z;

			materialsMap[activeMaterial].ambient[0] = x;
			materialsMap[activeMaterial].ambient[1] = y;
			materialsMap[activeMaterial].ambient[2] = z;
		}
		else if(start == "Ks") {

			float x,y,z;

			iss >> x >> y >> z;

			materialsMap[activeMaterial].specular[0] = x;
			materialsMap[activeMaterial].specular[1] = y;
			materialsMap[activeMaterial].specular[2] = z;
		}
		else if(start == "Ns") {

			float x;

			iss >> x;

			materialsMap[activeMaterial].specularConstant = x;
		}
		else if(start == "newmtl") {

			iss >> activeMaterial;
		}
	}
	materialFile.close();

	//first passage, just counting lines
	std::ifstream infile(LOCATION + modelFilename);

	while(getline(infile, line)) {

		istringstream iss(line);

		string start;
		iss >> start;
		
		if(start == "v")
			verticesN++;
		else if(start == "vt")
			uvcoordsN++;
		else if(start == "vn")
			normalsN++;
		else if(start == "f")
			facesN++;
	}

	infile.close();

	vertexCount = facesN * 3;

	std::ifstream infile2(LOCATION + modelFilename);

	GLfloat *vertices = new GLfloat[(verticesN+1) * 3 + 1];
	GLfloat *uvcoord = new GLfloat[(uvcoordsN+1) * 2 + 1];
	GLfloat *normals = new GLfloat[(normalsN+1) * 3 + 1];

	Vertex *bufferVertices = new Vertex[vertexCount];

	verticesN = 0; uvcoordsN = 0; normalsN = 0; facesN = 0;

	bool hasNormals = false;

	while(getline(infile2, line)) {

		istringstream iss(line);

		string start;

		iss >> start;

		float x,y,z;
		
		if(start == "v") {

			verticesN++;

			iss >> x >> y >> z;

			vertices[verticesN*3 + X] = x;
			vertices[verticesN*3 + Y] = y;
			vertices[verticesN*3 + Z] = z;			
		}
		else if(start == "vt") {

			uvcoordsN++;

			iss >> x >> y;

			uvcoord[uvcoordsN*2 + X] = x;
			uvcoord[uvcoordsN*2 + Y] = y;
		}
		else if(start == "vn") {

			normalsN++;

			iss >> x >> y >> z;

			normals[normalsN*3 + X] = x;
			normals[normalsN*3 + Y] = y;
			normals[normalsN*3 + Z] = z;			
		}
		else if(start == "usemtl") {

			iss >> activeMaterial;
		}
		else if(start == "f") {

			string a,b,c;
			iss >> a >> b >> c;

			vector<int> index;
			
			for(int i = 0; i < 3; i++) {

				if(i == 0)
					index = split(a, '/');
				if(i == 1)
					index = split(b, '/');
				if(i == 2)
					index = split(c, '/');
				
				bufferVertices[facesN * 3 + i].position[0] = vertices[(index[0] * 3) + X];
				bufferVertices[facesN * 3 + i].position[1] = vertices[(index[0] * 3) + Y];
				bufferVertices[facesN * 3 + i].position[2] = vertices[(index[0] * 3) + Z];
				bufferVertices[facesN * 3 + i].position[3] = 1.0f;

				if(!materials) {

					bufferVertices[facesN * 3 + i].ambient[0] = 1.0f;
					bufferVertices[facesN * 3 + i].ambient[1] = 1.0f;
					bufferVertices[facesN * 3 + i].ambient[2] = 1.0f;
					bufferVertices[facesN * 3 + i].ambient[3] = 1.0f;

					bufferVertices[facesN * 3 + i].diffuse[0] = 1.0f;
					bufferVertices[facesN * 3 + i].diffuse[1] = 1.0f;
					bufferVertices[facesN * 3 + i].diffuse[2] = 1.0f;
					bufferVertices[facesN * 3 + i].diffuse[3] = 1.0f;
					
					bufferVertices[facesN * 3 + i].specular[0] = 0.0f;
					bufferVertices[facesN * 3 + i].specular[1] = 0.0f;
					bufferVertices[facesN * 3 + i].specular[2] = 0.0f;
					bufferVertices[facesN * 3 + i].specular[3] = 1.0f;

					bufferVertices[facesN * 3 + i].specularConstant = 1.0f;

				}
				else {

					bufferVertices[facesN * 3 + i].ambient[0] = materialsMap[activeMaterial].ambient[0];
					bufferVertices[facesN * 3 + i].ambient[1] = materialsMap[activeMaterial].ambient[1];
					bufferVertices[facesN * 3 + i].ambient[2] = materialsMap[activeMaterial].ambient[2];
					bufferVertices[facesN * 3 + i].ambient[3] = 1.0f;

					bufferVertices[facesN * 3 + i].diffuse[0] = materialsMap[activeMaterial].diffuse[0];
					bufferVertices[facesN * 3 + i].diffuse[1] = materialsMap[activeMaterial].diffuse[1];
					bufferVertices[facesN * 3 + i].diffuse[2] = materialsMap[activeMaterial].diffuse[2];
					bufferVertices[facesN * 3 + i].diffuse[3] = 1.0f;
					
					bufferVertices[facesN * 3 + i].specular[0] = materialsMap[activeMaterial].specular[0];
					bufferVertices[facesN * 3 + i].specular[1] = materialsMap[activeMaterial].specular[1];
					bufferVertices[facesN * 3 + i].specular[2] = materialsMap[activeMaterial].specular[2];
					bufferVertices[facesN * 3 + i].specular[3] = 1.0f;
					
					bufferVertices[facesN * 3 + i].specularConstant = materialsMap[activeMaterial].specularConstant;
				}

				if(index.size() >= 3) {

					bufferVertices[facesN * 3 + i].normal[0] = normals[(index[2] * 3) + X];
					bufferVertices[facesN * 3 + i].normal[1] = normals[(index[2] * 3) + Y];
					bufferVertices[facesN * 3 + i].normal[2] = normals[(index[2] * 3) + Z];
					bufferVertices[facesN * 3 + i].normal[3] = 0.0f;
				}
				else {

					bufferVertices[facesN * 3 + i].normal[0] = 0.0f;
					bufferVertices[facesN * 3 + i].normal[1] = 0.0f;
					bufferVertices[facesN * 3 + i].normal[2] = 0.0f;
					bufferVertices[facesN * 3 + i].normal[3] = 0.0f; 				
				}
			
				if(index.size() >= 2) {

					bufferVertices[facesN * 3 + i].textureUV[0] = uvcoord[(index[1] * 2) + X];
					bufferVertices[facesN * 3 + i].textureUV[1] = uvcoord[(index[1] * 2) + Y];
				}
				else {

					bufferVertices[facesN * 3 + i].textureUV[0] = 0.0f;
					bufferVertices[facesN * 3 + i].textureUV[1] = 0.0f;
				}

				if(index.size() >= 3)
					hasNormals = true;

				bufferVertices[facesN * 3 + i].tangent[0] = 0.0f; 
				bufferVertices[facesN * 3 + i].tangent[1] = 0.0f; 
				bufferVertices[facesN * 3 + i].tangent[2] = 0.0f; 
				bufferVertices[facesN * 3 + i].tangent[3] = 1.0f; 
			}

			facesN++;
		}

		iss.clear();
	}

	BufferObject* bufferObject = new BufferObject(bufferVertices,vertexCount);
	bufferObject->createBufferObject();

	graphicObject->setBufferObject(bufferObject);

	delete vertices;
	delete uvcoord;
	delete normals;
	delete bufferVertices;
}

std::vector<int> &split(const std::string &s, char delim, std::vector<int> &elems) {

    std::stringstream ss(s);
    string item;

    while (std::getline(ss, item, delim)) {

		int i = atoi(item.c_str());
		elems.push_back(i);
    }

    return elems;
}

std::vector<int> split(const std::string &s, char delim) {

    std::vector<int> elems;
    split(s, delim, elems);

    return elems;
}