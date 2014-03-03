#include "Vector.h"

const GLfloat Vector::threshold = (GLfloat)1.0e-5;

Vector::Vector() {

	for(int i=0; i<3; i++)
		_vector[i] = 0.0f;

	_vector[VW] = 1.0f;
}

Vector::Vector(GLfloat initialValue) {

	for(int i=0; i<3; i++)
		_vector[i] = initialValue;

	_vector[VW] = 1.0f;
}

Vector::Vector(const GLfloat initialValue[4]) {

	for(int i=0; i<4; i++)
		_vector[i] = initialValue[i];
}

Vector::Vector(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {

	_vector[VX] = x;
	_vector[VY] = y;
	_vector[VZ] = z;
	_vector[VW] = w;
}

Vector::~Vector() {
}

void Vector::clean() {

	for(int i=0; i<3; i++)
		if(fabs(_vector[i]) < threshold) 
			_vector[i] = 0.0f;

	_vector[VW] = 1.0f;
}

void Vector::negate() {
	
	for(int i=0; i<4; i++)
		_vector[i] = -_vector[i];

	_vector[VW] = 1.0f;
}

void Vector::normalize() {
	
	GLfloat magnitude = sqrt(dotProduct(*this,*this));

	if(magnitude == 0)
		return;

	for(int i=0; i<3; i++)
		_vector[i] /= magnitude;

	_vector[VW] = 1.0f;
}

GLfloat Vector::magnitude() {

	return sqrt(dotProduct(*this,*this));
}

Vector Vector::projection(Vector u, Vector v) {

	v.normalize();

	Vector projection = v * dotProduct(u,v); 

	projection[VW] = 1.0f;

	return projection;
}

Vector Vector::crossProduct(Vector u, Vector v) {

	Vector crossProduct;

	crossProduct[VX] = u[VY] * v[VZ] - u[VZ] * v[VY];
	crossProduct[VY] = u[VZ] * v[VX] - u[VX] * v[VZ];
	crossProduct[VZ] = u[VX] * v[VY] - u[VY] * v[VX];

	crossProduct[VW] = 1.0f;

	return crossProduct; 
}

GLfloat Vector::dotProduct(Vector u, Vector v) {

	GLfloat internalProduct = 0.0f;

	for(int i=0; i<3; i++)
		internalProduct += u[i] * v[i];

	return internalProduct; 
}

void Vector::getValue(GLfloat* vector) {

	for(int i=0; i<4; i++)
		vector[i] = _vector[i];
}

void Vector::setValue(const GLfloat value[4]) {

	for(int i=0; i<4; i++)
		_vector[i] = value[i];
}

GLfloat& Vector::operator [] (int position){

	return _vector[position];
}

Vector Vector::operator - () {

	Vector result;

	for(int i=0; i<4; i++)
		result[i] = -_vector[i];

	result[VW] =  1.0f;

	return result;
}

Vector Vector::operator + (Vector vector){

	Vector result;

	for(int i=0; i<4; i++)
		result[i] = _vector[i] + vector[i];

	result[VW] =  1.0f;

	return result;
}

Vector Vector::operator += (Vector vector) {

	for(int i=0; i<4; i++)
		_vector[i] +=  vector[i];

	_vector[VW] =  1.0f;

	return (*this);
}

Vector Vector::operator - (Vector vector) {

	Vector result;

	for(int i=0; i<4; i++)
		result[i] = _vector[i] - vector[i];

	result[VW] =  1.0f;

	return result;
}

Vector Vector::operator-= (Vector vector) {

	for(int i=0; i<4; i++)
		_vector[i] -=  vector[i];

	_vector[VW] =  1.0f;

	return (*this);
}

Vector Vector::operator * (GLfloat scalar) {

	Vector result;

	for(int i=0; i<3; i++)
		result[i] = _vector[i] * scalar;

	_vector[VW] = 1.0f;

	return result;
}

Vector Vector::operator *= (GLfloat scalar) {

	for(int i=0; i<3; i++)
		_vector[i] *= scalar;

	_vector[VW] = 1.0f;

	return (*this);
}

bool Vector::operator== (Vector vector) {

	for(int i=0; i<3; i++)
		if(!(fabs(_vector[i] - vector[i]) < threshold))
			return false;

	return true;
}

void Vector::dump() {

	cout.precision(5);

	cout << "<Vector Dump> = ";

	for(int i=0; i<4; i++)
		cout << " [" << _vector[i] << "]";

	cout << endl;
}