#include "NFF_Reader.h"

NFF_Reader* NFF_Reader::instance = NULL;

NFF_Reader::NFF_Reader() {
}

NFF_Reader::~NFF_Reader() {
}

NFF_Reader* NFF_Reader::getInstance() {

	if(instance == NULL)
		instance = new NFF_Reader();

	return instance;
}

void NFF_Reader::destroyInstance() {

	delete instance;

	instance = NULL;
}


void NFF_Reader::loadModel(string modelFileName) {

	cout << "NFF_Reader::loadModel(" << modelFileName << ");" << endl;

	string line;

	/* Reading NFF file - First Pass */
	int numberLights = 0;
	int numberMaterialProperties = 0;
	int numberSurfaces = 0;
	int numberSpheres = 0;

	ifstream modelFile(LOCATION + modelFileName);

	while(getline(modelFile, line)) {

		istringstream iss(line);

		string start;
		iss >> start;

		if(start == "l")
			numberLights++;
		else if(start == "f")
			numberMaterialProperties++;
		else if(start == "pl")
			numberSurfaces++;
		else if(start == "s")
			numberSpheres++;
	}

	modelFile.close();

	/* Reading NFF file - Second Pass */
	modelFile.open(LOCATION + modelFileName);

	/* Storage Strucures */
	float backgroundColor[3] = {0}; //b

	float viewingLocation[3][3] = {0}; //v
	float angle = 0;
	float hither = 0;
	int resolution[2] = {0};

	Coordinate3D *lightLocation = new Coordinate3D[numberLights]; //l

	MaterialProperty *materialProperties = new MaterialProperty[numberMaterialProperties]; //f

	Coordinate3D *surface = new Coordinate3D[3]; //pl

	Sphere *sphereMap = new Sphere[numberSpheres]; //s

	/* Index Trackers */
	int currentLight = 0;
	int currentMaterial = 0;
	int currentSphere = 0;

	while(getline(modelFile, line)) {

		istringstream iss(line);

		string start;
		iss >> start;

		if(start == "b") {

			float r,g,b;
			iss >> r >> g >> b;

			backgroundColor[0] = r;
			backgroundColor[1] = g;
			backgroundColor[2] = b;

		}
		//v
		else if(start == "from") {
			
			float x,y,z;
			iss >> x >> y >> z;

			viewingLocation[0][0] = x;
			viewingLocation[0][1] = y;
			viewingLocation[0][2] = z;

		}
		else if(start == "at") {

			float x,y,z;
			iss >> x >> y >> z;

			viewingLocation[1][0] = x;
			viewingLocation[1][1] = y;
			viewingLocation[1][2] = z;

		}
		else if(start == "up") {

			float x,y,z;
			iss >> x >> y >> z;

			viewingLocation[2][0] = x;
			viewingLocation[2][1] = y;
			viewingLocation[2][2] = z;

		}
		else if(start == "angle") {

			iss >> angle;
		}
		else if(start == "hither") {

			iss >> hither;
		}
		else if(start == "resolution") {

			iss >> resolution[0] >> resolution[1];
		}
		else if(start == "l") {

			float x,y,z;
			float r,g,b;
			iss >> x >> y >> z >> r >> g >> b;

			lightLocation[currentLight].x = x;
			lightLocation[currentLight].y = y;
			lightLocation[currentLight].z = z;
		
			currentLight++;
		}
		else if(start == "f") {

			float r,g,b;
			float Kd, Ks;
			float shine,transmittance,indexOfRefraction;
			iss >> r >> g >> b >> Kd >> Ks >> shine >> transmittance >> indexOfRefraction;

			materialProperties[currentMaterial].r = r;
			materialProperties[currentMaterial].g = g;
			materialProperties[currentMaterial].b = b;
			materialProperties[currentMaterial].Kd = Kd;
			materialProperties[currentMaterial].Ks = Ks;
			materialProperties[currentMaterial].shine = shine;
			materialProperties[currentMaterial].transmittance = transmittance;
			materialProperties[currentMaterial].indexOfRefraction = indexOfRefraction;

			currentMaterial++;
		}
		else if(start == "pl"){

			float x1, y1, z1, x2, y2, z2, x3, y3, z3;
			iss >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

			surface[0].x = x1;
			surface[0].x = y1;
			surface[0].x = z1;

			surface[1].x = x2;
			surface[1].x = y2;
			surface[1].x = z2;

			surface[2].x = x3;
			surface[2].x = y3;
			surface[2].x = z3;

		}
		else if(start == "s") {

			float x,y,z,r;
			iss >> x >> y >> z >> r;

			sphereMap[currentSphere].centerX = x;
			sphereMap[currentSphere].centerY = y;
			sphereMap[currentSphere].centerZ = z;
			sphereMap[currentSphere].radius = r;

			currentSphere++;
		}
		iss.clear();
	}

	modelFile.close();
}