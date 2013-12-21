#include "BoundingBox.h"

BoundingBox::BoundingBox(GLfloat side) {

	_type.assign(BOUNDING_BOX);

	_height = side;
	_width = side;
	_depth = side;

	_position.setValue(0.0f, 0.0f, 0.0f, 1.0f);
}

BoundingBox::BoundingBox(GLfloat height, GLfloat width) {

	_type.assign(BOUNDING_BOX);

	_height = height;
	_width = width;
	_depth = 0.0f;

	_position.setValue(0.0f, 0.0f, 0.0f, 1.0f);
}

BoundingBox::BoundingBox(GLfloat height, GLfloat width, GLfloat depth) {

	_type.assign(BOUNDING_BOX);

	_height = height;
	_width = width;
	_depth = depth;

	_position.setValue(0.0f, 0.0f, 0.0f, 1.0f);
}

BoundingBox::~BoundingBox() {
}

void BoundingBox::update(GraphicObject* graphicObject) {

	BoundingVolume::update(graphicObject);

	_xAxis.setValue(1.0f,0.0f,0.0f,1.0f);
	_yAxis.setValue(0.0f,1.0f,0.0f,1.0f);
	_zAxis.setValue(0.0f,0.0f,1.0f,1.0f);

	Quaternion rotationQuaternion(graphicObject->getRotationQuaternion());
	Matrix rotationMatrix(rotationQuaternion);
	rotationMatrix.transpose();

	_xAxis = rotationMatrix*_xAxis;
	_xAxis.clean();
	_xAxis.normalize();

	_yAxis = rotationMatrix*_yAxis;
	_yAxis.clean();
	_yAxis.normalize();

	_zAxis = rotationMatrix*_zAxis;
	_zAxis.clean();
	_zAxis.normalize();

	_cornerMap[0] = rotationMatrix * Vector( _width, _depth,0.0f,1.0f);
	_cornerMap[0].clean();

	_cornerMap[1] = rotationMatrix * Vector( _width,-_depth,0.0f,1.0f);
	_cornerMap[1].clean();

	_cornerMap[2] = rotationMatrix * Vector(-_width,-_depth,0.0f,1.0f);
	_cornerMap[2].clean();

	_cornerMap[3] = rotationMatrix * Vector(-_width, _depth,0.0f,1.0f);
	_cornerMap[3].clean();
}

void BoundingBox::update(GraphicObject* graphicObject, GLfloat elapsedTime) {

	BoundingVolume::update(graphicObject,elapsedTime);

	_xAxis.setValue(1.0f,0.0f,0.0f,1.0f);
	_yAxis.setValue(0.0f,1.0f,0.0f,1.0f);
	_zAxis.setValue(0.0f,0.0f,1.0f,1.0f);

	Quaternion rotationQuaternion(graphicObject->getRotationQuaternion());
	Matrix rotationMatrix(rotationQuaternion);
	rotationMatrix.transpose();

	_xAxis = rotationMatrix*_xAxis;
	_xAxis.clean();
	_xAxis.normalize();

	_yAxis = rotationMatrix*_yAxis;
	_yAxis.clean();
	_yAxis.normalize();

	_zAxis = rotationMatrix*_zAxis;
	_zAxis.clean();
	_zAxis.normalize();

	// 2D
	_cornerMap[0] = rotationMatrix * Vector( _width, _depth,0.0f,1.0f);
	_cornerMap[0].clean();

	_cornerMap[1] = rotationMatrix * Vector( _width,-_depth,0.0f,1.0f);
	_cornerMap[1].clean();

	_cornerMap[2] = rotationMatrix * Vector(-_width,-_depth,0.0f,1.0f);
	_cornerMap[2].clean();

	_cornerMap[3] = rotationMatrix * Vector(-_width, _depth,0.0f,1.0f);
	_cornerMap[3].clean();
}

GLfloat BoundingBox::getHeight() {

	return _height;
}

GLfloat BoundingBox::getWidth() {

	return _width;
}

GLfloat BoundingBox::getDepth() {

	return _depth;
}

Vector BoundingBox::getXAxis() {

	return _xAxis;
}

Vector BoundingBox::getYAxis() {

	return _yAxis;
}

Vector BoundingBox::getZAxis() {

	return _zAxis;
}

Vector BoundingBox::getCorner(int cornerID){

	return _cornerMap[cornerID];
}

void BoundingBox::setHeight(GLfloat height) {

	_height = height;
}

void BoundingBox::setWidth(GLfloat width) {

	_width = width;
}

void BoundingBox::setDepth(GLfloat depth) {

	_depth = depth;
}

void BoundingBox::setXAxis(Vector xAxis) {

	_xAxis = xAxis;
}

void BoundingBox::setYAxis(Vector yAxis) {

	_yAxis = yAxis;
}

void BoundingBox::setZAxis(Vector zAxis) {

	_zAxis = zAxis;
}

void BoundingBox::addCorner(Vector corner, int cornerID) {

	_cornerMap[cornerID] = corner;
}

void BoundingBox::removeCorner(int cornerID) {

	_cornerMap.erase(cornerID);
}

void BoundingBox::dump() {

	cout << "<BoundingBox Dump>" << endl;

	/* Box Dimensions */
	cout << "<BoundingBox Height> = " << _height << " ;" << endl;
	cout << "<BoundingBox Width> = " << _width << " ;" << endl;
	cout << "<BoundingBox Depth> = " << _depth << " ;" << endl;

	_xAxis.dump();
	_yAxis.dump();
	_zAxis.dump();

	BoundingVolume::dump();
}