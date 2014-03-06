#include "Object.h"

Object::Object(string name) {

	_name = name;
	_parentName = "None";

	_modelMatrix.loadIdentity();

	_bufferObject = NULL;

	_scale = Vector(1.0f);

	_activeTexture = NO_MAPPING;
}

Object::~Object() {

	if(_bufferObject != NULL) {
	
		_bufferObject->destroyBufferObject();

		delete _bufferObject;
	}

	/* Destroy Texturess */
	map<string,Texture*>::const_iterator textureIterator;
	for(textureIterator = _textureMap.begin(); textureIterator != _textureMap.end(); textureIterator++)
		delete textureIterator->second;
}

void Object::draw(GLuint programID) {

	if(_bufferObject == NULL) {

		cerr << "BufferObject not initialized." << endl;
		return;
	}

	/* Bind the Shader Program */
	glBindVertexArray(_bufferObject->getVertexArrayObjectID());
	glUseProgram(programID);

	/* Get the Model Matrix */
	GLfloat modelMatrix[16];

	_modelMatrix.getValue(modelMatrix);

	/* Update the Model Matrix Uniform */
	glUniformMatrix4fv(glGetUniformLocation(programID,MODEL_MATRIX_UNIFORM), 1, GL_TRUE, modelMatrix);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" MODEL_MATRIX_UNIFORM "\" error.");

	/* Bind the Active Texture */
	switch(_activeTexture) {

		case BUMP_MAPPING:	bindBumpTexture(programID);
							break;

		case WOOD_MAPPING:	bindWoodTexture(programID);
							break;

		case MIXED_TEXTURE_MAPPING:	bindMixedTexture(programID);
									break;

		case SPHERE_MAPPING: bindSphereMapTexture(programID);
									break;

		default:			break;
	}

	/* Draw the Model */
	glDrawArrays(GL_TRIANGLES,0,_bufferObject->getVertexCount());

	/* Unbind the Active Texture */
	switch(_activeTexture) {
	
		case BUMP_MAPPING:	unbindBumpTexture();
							break;

		case WOOD_MAPPING:	unbindWoodTexture();
							break;

		case MIXED_TEXTURE_MAPPING:	unbindMixedTexture();
									break;

		case SPHERE_MAPPING: unbindSphereMapTexture();
									break;

		default:			break;
	}

	/* Unbind the Shader Program */
	glUseProgram(0);
	glBindVertexArray(0);
			
	Utility::checkOpenGLError("ERROR: Object drawing failed.");
}

void Object::update() {

	/* Update the Model Matrix */
	_modelMatrix.loadIdentity();

	_modelMatrix.translate(_position[0],_position[1],_position[2]);

	_modelMatrix.quaternionRotate(_rotationQuaternion);
}

void Object::update(GLfloat elapsedTime) {

	/* Update Position */
	for(int i=0; i<3; i++)
		_position[i] += _velocity[i]*elapsedTime;

	/* Update the Model Matrix */
	_modelMatrix.translate(_position[0],_position[1],_position[2]);
		
	_modelMatrix.quaternionRotate(_rotationQuaternion);
}

GLfloat Object::isIntersecting(Vector origin, Vector direction) {

	Vertex* vertices = _bufferObject->getVertices();

	Matrix modelMatrix = _modelMatrix;

	for(int i=0; i<_bufferObject->getVertexCount()/3; i++) {

		Vector vertex0(vertices[i*3+0].position[VX],vertices[i*3+0].position[VY],vertices[i*3+0].position[VZ],1.0f);
		Vector vertex1(vertices[i*3+1].position[VX],vertices[i*3+1].position[VY],vertices[i*3+1].position[VZ],1.0f);
		Vector vertex2(vertices[i*3+2].position[VX],vertices[i*3+2].position[VY],vertices[i*3+2].position[VZ],1.0f);

		vertex0 = modelMatrix * vertex0;
		vertex1 = modelMatrix * vertex1;
		vertex2 = modelMatrix * vertex2;

		Vector edge1 = vertex1;
		edge1 -= vertex0;

		Vector edge2 = vertex2;		
		edge2 -= vertex0;

		Vector p = Vector::crossProduct(direction,edge2);

		GLfloat determinant = Vector::dotProduct(edge1,p);

		if(fabs(determinant) < Vector::threshold)
			continue;

		GLfloat invertedDeterminant = 1.0f / determinant;
 
		Vector t = origin;
		t -= vertex0;
 
		GLfloat u = Vector::dotProduct(t,p) * invertedDeterminant;

		if(u < 0.0f || u > 1.0f)
			continue;

		Vector q = Vector::crossProduct(t,edge1);
 
		GLfloat v = Vector::dotProduct(direction,q) * invertedDeterminant;

		if(v < 0.0f || u + v  > 1.0f)
			continue;
 
		GLfloat w = Vector::dotProduct(edge2,q) * invertedDeterminant;
 
		if(w > Vector::threshold)
			return w;
	}

	return 0.0f;
}

void Object::activateBumpTexture(string diffuseTextureFileName, string normalTextureFileName) {

	_activeTexture = BUMP_MAPPING;

	_textureMap[DIFFUSE_TEXTURE] = new Texture(GL_TEXTURE_2D, diffuseTextureFileName);
	_textureMap[DIFFUSE_TEXTURE]->load();

	_textureMap[NORMAL_TEXTURE] = new Texture(GL_TEXTURE_2D, normalTextureFileName);
	_textureMap[NORMAL_TEXTURE]->load();
}

void Object::bindBumpTexture(GLuint programID) { 

	Texture* diffuseTexture = _textureMap[DIFFUSE_TEXTURE];
	diffuseTexture->bind(GL_TEXTURE0);

	glProgramUniform1i(programID, glGetUniformLocation(programID, DIFFUSE_TEXTURE_UNIFORM), 0);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" + diffuseTexture->getFileName() + "\" error.");

	Texture* normalTexture = _textureMap[NORMAL_TEXTURE];				
	normalTexture->bind(GL_TEXTURE1);

	glProgramUniform1i(programID, glGetUniformLocation(programID, NORMAL_TEXTURE_UNIFORM), 1);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" + normalTexture->getFileName() + "\" error.");
}

void Object::unbindBumpTexture() {

	Texture* diffuseTexture = _textureMap[DIFFUSE_TEXTURE];
	diffuseTexture->unbind(GL_TEXTURE0);

	Texture* normalTexture = _textureMap[NORMAL_TEXTURE];
	normalTexture->unbind(GL_TEXTURE1);
}

void Object::activateWoodTexture(GLfloat noiseAlpha, GLfloat noiseBeta, GLint noiseOctaves, GLfloat noiseScale) {

	_activeTexture = WOOD_MAPPING;

	_textureMap[WOOD_TEXTURE] = new GeneratedTexture(GL_TEXTURE_3D,noiseAlpha,noiseBeta,noiseOctaves,noiseScale);
	_textureMap[WOOD_TEXTURE]->load();
}

void Object::bindWoodTexture(GLuint programID) { 

	GeneratedTexture* woodTexture = (GeneratedTexture*)_textureMap[WOOD_TEXTURE];		
	woodTexture->bind(GL_TEXTURE0);

	glProgramUniform1i(programID, glGetUniformLocation(programID, NOISE_TEXTURE_UNIFORM), 0);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" NOISE_TEXTURE_UNIFORM "\" error.");

	glProgramUniform1f(programID,glGetUniformLocation(programID,NOISE_SCALE_UNIFORM),woodTexture->getNoiseScale());
	Utility::checkOpenGLError("ERROR: Uniform Location \"" NOISE_SCALE_UNIFORM "\" error.");
}

void Object::unbindWoodTexture() {

	Texture* woodTexture = _textureMap[WOOD_TEXTURE];				
	woodTexture->unbind(GL_TEXTURE0);
}

void Object::activateMixedTexture(string texture0FileName, string texture1FileName) {

	_activeTexture = MIXED_TEXTURE_MAPPING;

	_textureMap[MIXED_TEXTURE_0] = new Texture(GL_TEXTURE_2D, texture0FileName);
	_textureMap[MIXED_TEXTURE_0]->load();

	_textureMap[MIXED_TEXTURE_1] = new Texture(GL_TEXTURE_2D, texture1FileName);
	_textureMap[MIXED_TEXTURE_1]->load();
}

void Object::bindMixedTexture(GLuint programID) {

	Texture* diffuseTexture = _textureMap[MIXED_TEXTURE_0];
	diffuseTexture->bind(GL_TEXTURE0);

	glProgramUniform1i(programID, glGetUniformLocation(programID, MIXED_TEXTURE_0_UNIFORM), 0);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" + diffuseTexture->getFileName() + "\" error.");

	Texture* normalTexture = _textureMap[MIXED_TEXTURE_1];				
	normalTexture->bind(GL_TEXTURE1);

	glProgramUniform1i(programID, glGetUniformLocation(programID, MIXED_TEXTURE_1_UNIFORM), 1);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" + normalTexture->getFileName() + "\" error.");
}

void Object::unbindMixedTexture() {

	Texture* diffuseTexture = _textureMap[MIXED_TEXTURE_0];
	diffuseTexture->unbind(GL_TEXTURE0);

	Texture* normalTexture = _textureMap[MIXED_TEXTURE_1];
	normalTexture->unbind(GL_TEXTURE1);
}


void Object::activateSphereMapTexture(string sphereMapFileName) {

	_activeTexture = SPHERE_MAPPING;

	_textureMap[SPHERE_MAP_TEXTURE] = new Texture(GL_TEXTURE_2D, sphereMapFileName);
	_textureMap[SPHERE_MAP_TEXTURE]->load();
}

void Object::bindSphereMapTexture(GLuint programID) {

	Texture* sphereMapTexture = _textureMap[SPHERE_MAP_TEXTURE];
	sphereMapTexture->bind(GL_TEXTURE0);

	glProgramUniform1i(programID, glGetUniformLocation(programID, SPHERE_MAP_UNIFORM), 0);
	Utility::checkOpenGLError("ERROR: Uniform Location \"" + sphereMapTexture->getFileName() + "\" error.");
}

void Object::unbindSphereMapTexture() {

	Texture* sphereMapTexture = _textureMap[SPHERE_MAP_TEXTURE];
	sphereMapTexture->unbind(GL_TEXTURE0);
}

void Object::deactivateTexture() {

	/* Destroy Texturess */
	map<string,Texture*>::const_iterator textureIterator;
	for(textureIterator = _textureMap.begin(); textureIterator != _textureMap.end(); textureIterator++)
		delete textureIterator->second;

	_activeTexture = NO_MAPPING;
}

string Object::getName() {

	return _name;
}

string Object::getParentName() {

	return _parentName;
}

Matrix Object::getModelMatrix() {

	return _modelMatrix;
}

BufferObject* Object::getBufferObject() {

	return _bufferObject;
}

Vector Object::getPosition() {

	return _position;
}

Vector Object::getVelocity() {

	return _velocity;
}

Vector Object::getScale() {

	return _scale;
}

Vector Object::getRotation() {

	return _rotation;
}

Quaternion Object::getRotationQuaternion() {

	return _rotationQuaternion;
}

void Object::setName(string name) {

	_name = name;
}

void Object::setParentName(string parentName) {

	_parentName = parentName;
}

void Object::setModelMatrix(Matrix modelMatrix) {

	_modelMatrix = modelMatrix;
}

void Object::setBufferObject(BufferObject* bufferObject) {

	_bufferObject = bufferObject;
}

void Object::setPosition(Vector position) {

	_position = position;
}
void Object::setVelocity(Vector velocity) {

	_velocity = velocity;
}

void Object::setScale(Vector scale) {

	_scale = scale;
}

void Object::setRotation(Vector rotation) {

	_rotation = rotation;

	GLfloat xAxis[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat yAxis[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat zAxis[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	Quaternion xQuaternion(_rotation[0],xAxis);
	Quaternion yQuaternion(_rotation[1],yAxis);
	Quaternion zQuaternion(_rotation[2],zAxis);

	_rotationQuaternion = xQuaternion;
	_rotationQuaternion *= yQuaternion;
	_rotationQuaternion *= zQuaternion;
}

void Object::setRotationQuaternion(Quaternion rotationQuaternion) {

	_rotationQuaternion = rotationQuaternion;
}

void Object::dump() {

	cout << "<Object \"" << _name << "\" Dump>" << endl;

	/* Graphic Object Model Matrix */
	cout << "<Object Model Matrix> = " << endl;
	_modelMatrix.dump();

	/* Graphic Object Scale Factor*/
	cout << "<Object Rotation> = ";
	_scale.dump();

	/* Graphic Object Rotation */
	cout << "<Object Rotation> = ";
	_rotation.dump();

	/* Graphic Object Position & Velocity*/
	cout << "<Object Position> = ";
	_position.dump();

	cout << "<Object Velocity> = ";
	_velocity.dump();

	/* Graphic Object BufferObject */
	_bufferObject->dump();
}