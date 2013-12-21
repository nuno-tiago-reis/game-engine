#include "OBJ_Reader.h"

OBJ_Reader::OBJ_Reader(){

}

OBJ_Reader::~OBJ_Reader(){

}

void OBJ_Reader::readFile(string path, GraphicObject* graphicObject){
	
	string line;
	string activeMaterial;
	int facesN = 0, verticesN = 0, uvcoordsN = 0, normalsN = 0, vertexCount = 0;
	bool materials = false;

	//first read all materials
	map<string,Material> materialsMap;
	std::ifstream materialFile(LOCATION + path + ".mtl");
	while(getline(materialFile, line)){
		materials = true;
		istringstream iss(line);
		string start;
		iss >> start;
		
		if(start == "Kd"){
			float x,y,z;
			iss >> x >> y >> z;
			materialsMap[activeMaterial].DIFUSE[0] = x;
			materialsMap[activeMaterial].DIFUSE[1] = y;
			materialsMap[activeMaterial].DIFUSE[2] = z;
		}

		else if(start == "Ka"){
			float x,y,z;
			iss >> x >> y >> z;
			materialsMap[activeMaterial].AMBIENT[0] = x;
			materialsMap[activeMaterial].AMBIENT[1] = y;
			materialsMap[activeMaterial].AMBIENT[2] = z;
		}

		else if(start == "Ks"){
			float x,y,z;
			iss >> x >> y >> z;
			materialsMap[activeMaterial].SPECULAR[0] = x;
			materialsMap[activeMaterial].SPECULAR[1] = y;
			materialsMap[activeMaterial].SPECULAR[2] = z;
		}

		else if(start == "Ns"){
			float x;
			iss >> x;
			materialsMap[activeMaterial].SPECULAR[3] = x;
		}

		else if(start == "newmtl"){
			iss >> activeMaterial;
		}

	}
	materialFile.close();

	//first passage, just counting lines
	std::ifstream infile(LOCATION + path + ".obj");
	while(getline(infile, line)){
		istringstream iss(line);
		string start;
		iss >> start;
		
		if(start == "v"){
			verticesN++;
		}

		else if(start == "vt"){
			uvcoordsN++;
		}

		else if(start == "vn"){
			normalsN++;
		}

		else if(start == "f"){
			facesN++;
		}
	}
	infile.close();

	vertexCount = facesN * 3;

	//second passage, now we actualy read the file
	std::ifstream infile2(LOCATION + path + ".obj");
	GLfloat *vertices = new GLfloat[(verticesN+1) * 3 + 1];
	GLfloat *uvcoord = new GLfloat[(uvcoordsN+1) * 2 + 1];
	GLfloat *normals = new GLfloat[(normalsN+1) * 3 + 1];
	Vertex *bufferVertices = new Vertex[vertexCount];
	verticesN = 0; uvcoordsN = 0; normalsN = 0; facesN = 0;
	bool hasNormals = false;

	while(getline(infile2, line)){
		istringstream iss(line);
		string start;
		iss >> start;
		float x,y,z;
		
		if(start == "v"){
			verticesN++;
			iss >> x >> y >> z;
			vertices[verticesN*3 + X] = x;
			vertices[verticesN*3 + Y] = y;
			vertices[verticesN*3 + Z] = z;			
		}

		else if(start == "vt"){
			uvcoordsN++;
			iss >> x >> y;
			uvcoord[uvcoordsN*2 + X] = x;
			uvcoord[uvcoordsN*2 + Y] = y;
		}

		else if(start == "vn"){
			normalsN++;
			iss >> x >> y >> z;
			normals[normalsN*3 + X] = x;
			normals[normalsN*3 + Y] = y;
			normals[normalsN*3 + Z] = z;			
		}

		else if(start == "usemtl"){
			iss >> activeMaterial;
		}

		else if(start == "f"){
			string a,b,c;
			iss >> a >> b >> c;

			vector<int> index;
			
			for(int i = 0; i < 3; i++){
				if(i == 0)
					index = split(a, '/');
				if(i == 1)
					index = split(b, '/');
				if(i == 2)
					index = split(c, '/');
				
				bufferVertices[facesN * 3 + i].XYZW[0] = vertices[(index[0] * 3) + X];
				bufferVertices[facesN * 3 + i].XYZW[1] = vertices[(index[0] * 3) + Y];
				bufferVertices[facesN * 3 + i].XYZW[2] = vertices[(index[0] * 3) + Z];
				bufferVertices[facesN * 3 + i].XYZW[3] = 1.0f; //W

				if(!materials){
					bufferVertices[facesN * 3 + i].RGBA[0] = 0.0f;
					bufferVertices[facesN * 3 + i].RGBA[1] = 0.0f;
					bufferVertices[facesN * 3 + i].RGBA[2] = 1.0f;
					bufferVertices[facesN * 3 + i].RGBA[3] = 1.0f;

					bufferVertices[facesN * 3 + i].AMBIENT[0] = 1.0f;
					bufferVertices[facesN * 3 + i].AMBIENT[1] = 1.0f;
					bufferVertices[facesN * 3 + i].AMBIENT[2] = 1.0f;
					bufferVertices[facesN * 3 + i].AMBIENT[3] = 1.0f;
					
					bufferVertices[facesN * 3 + i].SPECULAR[0] = 0.0f;
					bufferVertices[facesN * 3 + i].SPECULAR[1] = 0.0f;
					bufferVertices[facesN * 3 + i].SPECULAR[2] = 0.0f;
					bufferVertices[facesN * 3 + i].SPECULAR[3] = 1.0f;
					
					bufferVertices[facesN * 3 + i].SPECULAR_CONSTANT[0] = 1.0f;
				}else{
					bufferVertices[facesN * 3 + i].AMBIENT[0] = materialsMap[activeMaterial].AMBIENT[0];
					bufferVertices[facesN * 3 + i].AMBIENT[1] = materialsMap[activeMaterial].AMBIENT[1];
					bufferVertices[facesN * 3 + i].AMBIENT[2] = materialsMap[activeMaterial].AMBIENT[2];
					bufferVertices[facesN * 3 + i].AMBIENT[3] = 1.0f;

					bufferVertices[facesN * 3 + i].RGBA[0] = materialsMap[activeMaterial].DIFUSE[0];
					bufferVertices[facesN * 3 + i].RGBA[1] = materialsMap[activeMaterial].DIFUSE[1];
					bufferVertices[facesN * 3 + i].RGBA[2] = materialsMap[activeMaterial].DIFUSE[2];
					bufferVertices[facesN * 3 + i].RGBA[3] = 1.0f;
					
					bufferVertices[facesN * 3 + i].SPECULAR[0] = materialsMap[activeMaterial].SPECULAR[0];
					bufferVertices[facesN * 3 + i].SPECULAR[1] = materialsMap[activeMaterial].SPECULAR[1];
					bufferVertices[facesN * 3 + i].SPECULAR[2] = materialsMap[activeMaterial].SPECULAR[2];
					bufferVertices[facesN * 3 + i].SPECULAR[3] = 1.0f;
					
					bufferVertices[facesN * 3 + i].SPECULAR_CONSTANT[0] = materialsMap[activeMaterial].SPECULAR[3];
				}

				if(index.size() >= 3){
					bufferVertices[facesN * 3 + i].NORMAL[0] = normals[(index[2] * 3) + X];
					bufferVertices[facesN * 3 + i].NORMAL[1] = normals[(index[2] * 3) + Y];
					bufferVertices[facesN * 3 + i].NORMAL[2] = normals[(index[2] * 3) + Z];
					bufferVertices[facesN * 3 + i].NORMAL[3] = 0.0f; //W
				}
				//there are no normals in the file
				else{
					bufferVertices[facesN * 3 + i].NORMAL[0] = 0.0f;
					bufferVertices[facesN * 3 + i].NORMAL[1] = 0.0f;
					bufferVertices[facesN * 3 + i].NORMAL[2] = 0.0f;
					bufferVertices[facesN * 3 + i].NORMAL[3] = 0.0f; 				
				}
			
				if(index.size() >= 2){
					bufferVertices[facesN * 3 + i].TEXUV[0] = uvcoord[(index[1] * 2) + X];
					bufferVertices[facesN * 3 + i].TEXUV[1] = uvcoord[(index[1] * 2) + Y];
				}

				//ther are no uvs in the file
				else{
					bufferVertices[facesN * 3 + i].TEXUV[0] = 0.0f;
					bufferVertices[facesN * 3 + i].TEXUV[1] = 0.0f;
				}

				if(index.size() >= 3){
					hasNormals = true;
				}

				bufferVertices[facesN * 3 + i].TANGENT[0] = 0.0f; 
				bufferVertices[facesN * 3 + i].TANGENT[1] = 0.0f; 
				bufferVertices[facesN * 3 + i].TANGENT[2] = 0.0f; 
				bufferVertices[facesN * 3 + i].TANGENT[3] = 1.0f; 
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

//from stack overflow (modified), splits string on token
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