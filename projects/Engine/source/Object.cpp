#include "Object.h"

Object::Object(string name) {

	_name = name;
	_parentName = "None";

	_modelMatrix.loadIdentity();

	_bufferObject = NULL;

	_activeTexture = NO_MAPPING;
}

Object::~Object() {

	if(_bufferObject != NULL) {
	
		_bufferObject->destroyBufferObject();

		delete _bufferObject;
	}
}

void Object::draw(GLuint programID) {

	if(_bufferObject == NULL) {

		cerr << "BufferObject not initialized." << endl;
		return;
	}

	if(_name.compare("Platform") == 0)
		return;

	/* Bind the Shader Program */
	glBindVertexArray(_bufferObject->getVertexArrayObjectID());
	glUseProgram(programID);

	/* Update the Model Matrix Uniform */
	glUniformMatrix4fv(glGetUniformLocation(programID,MODEL_MATRIX_UNIFORM), 1, GL_TRUE, _modelMatrix.getValue());
	Utility::checkOpenGLError("ERROR: Uniform Location \"" MODEL_MATRIX_UNIFORM "\" error.");

	/* Bind the Active Texture */
	switch(_activeTexture) {

		case BUMP_MAPPING:	bindBumpTexture(programID);
							break;

		case WOOD_MAPPING:	bindWoodTexture(programID);
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

		default:			break;
	}

	/* Unbind the Shader Program */
	glUseProgram(0);
	glBindVertexArray(0);
			
	Utility::checkOpenGLError("ERROR: Object drawing failed.");
}

void Object::update() {

	/* Update Rotation */
	Quaternion rotationQuaternion(_rotationQuaternion.getValue());

	/* Update the Model Matrix */
	_modelMatrix.translate(_position[0],_position[1],_position[2]);

	_modelMatrix.quaternionRotate(rotationQuaternion);
}

void Object::update(GLfloat elapsedTime) {

	/* Update Position */
	for(int i=0; i<3; i++)
		_position[i] += _velocity[i]*elapsedTime;

	/* Update Rotation */
	Quaternion rotationQuaternion(_rotationQuaternion.getValue());

	/* Update the Model Matrix */
	_modelMatrix.translate(_position[0],_position[1],_position[2]);
		
	_modelMatrix.quaternionRotate(rotationQuaternion);
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

		Vector edge1(vertex1.getValue());
		Vector edge2(vertex2.getValue());

		edge1 -= vertex0;
		edge2 -= vertex0;

		Vector p = Vector::crossProduct(direction,edge2);

		GLfloat determinant = Vector::dotProduct(edge1,p);

		if(fabs(determinant) < Vector::threshold)
			continue;

		GLfloat invertedDeterminant = 1.0f / determinant;
 
		Vector t(origin.getValue());	

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

void Object::deactivateTexture() {

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

	_rotationQuaternion.setValue(xQuaternion.getValue());
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