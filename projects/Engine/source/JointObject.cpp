#include "JointObject.h"

JointObject::JointObject(string name) : Object(name) {
}

JointObject::~JointObject() {
}

void JointObject::draw(GLuint programID) {

	Object::draw(programID);
}

void JointObject::update() {

	/* Update the Model Matrix */
	_modelMatrix.translate(_position[0],_position[1],_position[2]);

	_modelMatrix.quaternionRotate(_rotationQuaternion);

	_modelMatrix.translate(_jointPosition[0],_jointPosition[1],_jointPosition[2]);
}

void JointObject::update(GLfloat elapsedTime) {

	/* Update Position */
	for(int i=0; i<3; i++)
		_position[i] += _velocity[i]*elapsedTime;

	/* Update the Model Matrix */
	_modelMatrix.translate(_position[0],_position[1],_position[2]);
		
	_modelMatrix.quaternionRotate(_rotationQuaternion);

	_modelMatrix.translate(_jointPosition[0],_jointPosition[1],_jointPosition[2]);
}

Vector JointObject::getJointPosition() {

	return _jointPosition;
}

void JointObject::setJointPosition(Vector jointPosition) {

	_jointPosition = jointPosition;
}

void JointObject::dump() {

	cout << "<JointObject \"" << _name << "\" Dump>" << endl;

	Object::dump();
}