#include "GraphicObject.h"

GraphicObject::GraphicObject(const char* name) {

	_name.assign(name);
	_parentName.assign("None");

	_modelMatrix.loadIdentity();

	_bufferObject = NULL;
	bumpActive = false;
	woodActive=false;

	_bounce = 1.0f;
	_friction = 1.0f;
	_boundingVolume = NULL;

	for(int i=0; i<3; i++) {

		_scale[i] = 1.0f;
		
		_rotation[i] = 0.0f;

		_position[i] = 0.0f;
	}
}

GraphicObject::~GraphicObject() {

	if(_bufferObject != NULL) {
	
		_bufferObject->destroyBufferObject();

		delete _bufferObject;
	}
}

void GraphicObject::activateBump(const char* diffuse_path, const char* normal_path){
	bumpActive = true;
		normalTexture = new Texture(GL_TEXTURE_2D, normal_path);
	normalTexture->load();

	diffuseTexture = new Texture(GL_TEXTURE_2D, diffuse_path);
	diffuseTexture->load();
}


void GraphicObject::draw(GLuint programID, GLuint uniformID) {

	if(_bufferObject == NULL) {

		cerr << "BufferObject not initialized." << endl;
		return;
	}


	/* Bind the Shader Program */
	glBindVertexArray(_bufferObject->getVertexArrayObjectID());
	glUseProgram(programID);

	glUniformMatrix4fv(uniformID, 1, GL_TRUE, _modelMatrix.getValue());


	if(bumpActive){

		diffuseTexture->bind(GL_TEXTURE0);
		glProgramUniform1i(programID, glGetUniformLocation(programID, "diffuse_tex"), 0);
		Utility::checkOpenGLError("ERROR: Uniform Location \"diffuse_tex\" error.");

		normalTexture->bind(GL_TEXTURE1);
		glProgramUniform1i(programID, glGetUniformLocation(programID, "normal_tex"), 1);
		Utility::checkOpenGLError("ERROR: Uniform Location \"normal_tex\" error.");
	
	}else if(woodActive){
		noiseTexture->bind(GL_TEXTURE0);
		glProgramUniform1i(programID, glGetUniformLocation(programID, "Noise"), 0);
		Utility::checkOpenGLError("ERROR: Uniform Location \"Noise\" error.");
		glProgramUniform1f(programID,glGetUniformLocation(programID,"noiseScale"),noiseScale);
	}
	glDrawArrays(GL_TRIANGLES,0,_bufferObject->getVertexCount());

	if(bumpActive){
		diffuseTexture->unbind(GL_TEXTURE0);
		normalTexture->unbind(GL_TEXTURE1);
	}else if(woodActive){
		noiseTexture->unbind(GL_TEXTURE0);
	}
	/* Unbind the Shader Program */
	glUseProgram(0);
	glBindVertexArray(0);
			
	Utility::checkOpenGLError("ERROR: GraphicObject drawing failed.");
}

void GraphicObject::update() {

	/* Update Rotation */
	Quaternion rotationQuaternion(_rotationQuaternion.getValue());

	/* Update the Model Matrix */
	_modelMatrix.translate(_position[0],_position[1],_position[2]);

	_modelMatrix.quaternionRotate(rotationQuaternion);

	_modelMatrix.scale(_scale[0],_scale[1],_scale[2]);
}

void GraphicObject::update(GLfloat elapsedTime) {

	/* Update Position */
	for(int i=0; i<3; i++)
		_position[i] += _velocity[i]*elapsedTime;

	/* Update Rotation */
	Quaternion rotationQuaternion(_rotationQuaternion.getValue());

	/* Update the Model Matrix */
	_modelMatrix.translate(_position[0],_position[1],_position[2]);

	_modelMatrix.scale(_scale[0],_scale[1],_scale[2]);
		
	_modelMatrix.quaternionRotate(rotationQuaternion);
}

void GraphicObject::updateCollision() {

	if(_boundingVolume == NULL)
		return;

	/* Update Bounding Volume */
	_boundingVolume->update(this);	
}

void GraphicObject::updateBoundingVolume(GLfloat elapsedTime) {

	if(_boundingVolume == NULL)
		return;

	/* Update Bounding Volume */
	_boundingVolume->update(this,elapsedTime);
}

GLfloat GraphicObject::isIntersecting(Vector origin, Vector direction) {

	Vertex* vertices = _bufferObject->getVertices();

	Matrix modelMatrix = _modelMatrix;

	for(int i=0; i<_bufferObject->getVertexCount()/3; i++) {

		Vector vertex0(vertices[i*3+0].XYZW[VX],vertices[i*3+0].XYZW[VY],vertices[i*3+0].XYZW[VZ],1.0f);
		Vector vertex1(vertices[i*3+1].XYZW[VX],vertices[i*3+1].XYZW[VY],vertices[i*3+1].XYZW[VZ],1.0f);
		Vector vertex2(vertices[i*3+2].XYZW[VX],vertices[i*3+2].XYZW[VY],vertices[i*3+2].XYZW[VZ],1.0f);

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

string GraphicObject::getName() {

	return _name;
}

string GraphicObject::getParentName() {

	return _parentName;
}

Matrix GraphicObject::getModelMatrix() {

	return _modelMatrix;
}

BufferObject* GraphicObject::getBufferObject() {

	return _bufferObject;
}

GLfloat GraphicObject::getBounce() {

	return _bounce;
}

GLfloat GraphicObject::getFriction() {

	return _friction;
}

BoundingVolume* GraphicObject::getBoundingVolume() {

	return _boundingVolume;
}

Vector GraphicObject::getScale() {

	return _scale;
}

Vector GraphicObject::getRotation() {

	return _rotation;
}

Quaternion GraphicObject::getRotationQuaternion() {

	return _rotationQuaternion;
}

Vector GraphicObject::getPosition() {

	return _position;
}

Vector GraphicObject::getVelocity() {

	return _velocity;
}

void GraphicObject::setName(const char* name) {

	_name.assign(name);
}

void GraphicObject::setParentName(const char* parentName) {

	_parentName.assign(parentName);
}

void GraphicObject::setModelMatrix(Matrix modelMatrix) {

	_modelMatrix = modelMatrix;
}

void GraphicObject::setBufferObject(BufferObject* bufferObject) {

	_bufferObject = bufferObject;
}

void GraphicObject::setBounce(GLfloat bounce) {

	_bounce = bounce;
}

void GraphicObject::setFriction(GLfloat friction) {

	_friction = friction;
}

void GraphicObject::setBoundingVolume(BoundingVolume* boundingVolume) {

	_boundingVolume = boundingVolume;
}

void GraphicObject::setScale(Vector scale) {

	_scale = scale;
}

void GraphicObject::setRotation(Vector rotation) {

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

void GraphicObject::setPosition(Vector position) {

	_position = position;
}
void GraphicObject::setVelocity(Vector velocity) {

	_velocity = velocity;
}

void GraphicObject::activateWood(GLfloat alpha, GLfloat beta, GLfloat octaves, GLfloat scale){
	if(bumpActive)bumpActive=false;
	woodActive=true;
	noiseScale=scale;
	noiseTexture=new GeneratedTexture(GL_TEXTURE_3D,alpha,beta,octaves);
	((GeneratedTexture*)noiseTexture)->generateNoiseTexture();
	Utility::checkOpenGLError("ERROR: Could not set Object as wooden.");
}

void GraphicObject::dump() {

	cout << "<GraphicObject \"" << _name << "\" Dump>" << endl;

	/* Graphic Object Model Matrix */
	cout << "<GraphicObject Model Matrix> = " << endl;
	_modelMatrix.dump();

	/* Graphic Object Scale Factor*/
	cout << "<GraphicObject Rotation> = ";
	_scale.dump();

	/* Graphic Object Rotation */
	cout << "<GraphicObject Rotation> = ";
	_rotation.dump();

	/* Graphic Object Position & Velocity*/
	cout << "<GraphicObject Position> = ";
	_position.dump();

	cout << "<GraphicObject Velocity> = ";
	_velocity.dump();

	/* Graphic Object BufferObject */
	_bufferObject->dump();

	/* Graphic Object BoundingVolume */
	_boundingVolume->dump();
}