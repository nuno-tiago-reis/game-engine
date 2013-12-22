#include "Quaternion.h"

const GLfloat Quaternion::threshold = (GLfloat)1.0e-5;

Quaternion::Quaternion() {

	_quaternion[QT] = 0.0f;
	_quaternion[QX] = 0.0f;
	_quaternion[QY] = 0.0f;
	_quaternion[QZ] = 0.0f;
}

Quaternion::Quaternion(GLfloat* initialValue) {

	for(int i=0; i<4; i++)
		_quaternion[i] = initialValue[i];

	clean();
	normalize();
}

Quaternion::Quaternion(GLfloat theta, Vector axis) {

	axis.normalize();

	GLfloat a = theta * (GLfloat)DEGREES_TO_RADIANS;
	GLfloat scalar = sin(a / 2.0f);

	_quaternion[QT] = cos(a / 2.0f);

	_quaternion[QX] = axis[VX] * scalar;
	_quaternion[QY] = axis[VY] * scalar;
	_quaternion[QZ] = axis[VZ] * scalar;

	clean();
	normalize();
}

Quaternion::~Quaternion() {
}

void Quaternion::toAngle(GLfloat* theta, Vector* axis) {		//const void qToAngleAxix(const Quaternion& q, float& theta, Vector& axis);

	normalize();

	*theta = 2.0f*(float)acos((float)_quaternion[QT])*(float)RADIANS_TO_DEGREES;

	GLfloat s = sqrt(1.0f - _quaternion[QT]*_quaternion[QT]);

	if(s < threshold) {

		(*axis)[VX] = 1.0f;
		(*axis)[VY] = 0.0f;
		(*axis)[VZ] = 0.0f;
		(*axis)[VW] = 1.0f;

	} else {

		(*axis)[VX] = _quaternion[QX]/s;
		(*axis)[VY] = _quaternion[QY]/s;
		(*axis)[VZ] = _quaternion[QZ]/s;
		(*axis)[VW] = 1.0f;
	}
}

void Quaternion::clean() {							

	if(fabs(_quaternion[QT]) < threshold)
		_quaternion[QT] = 0.0f;

	if(fabs(_quaternion[QX]) < threshold)
		_quaternion[QX] = 0.0f;

	if(fabs(_quaternion[QY]) < threshold)
		_quaternion[QY] = 0.0f;

	if(fabs(_quaternion[QZ]) < threshold)
		_quaternion[QZ] = 0.0f;
}

void Quaternion::invert() {							
	GLfloat q = quadrance();

	conjugate();

	(*this) *= (1.0f / q);
}

void Quaternion::conjugate() {								

	_quaternion[QX] = -_quaternion[QX];
	_quaternion[QY]= -_quaternion[QY];
	_quaternion[QZ] = -_quaternion[QZ];
}

void Quaternion::normalize() {								

	float scalar = 1 / norm();

	(*this) *= scalar;
}

GLfloat Quaternion::norm() {								

	return sqrt(quadrance());
}

GLfloat Quaternion::quadrance() {							

	GLfloat quadrance = 0.0f;

	for(int i=0; i<4; i++)
		quadrance += _quaternion[i]*_quaternion[i];

	return quadrance;
}

GLfloat* Quaternion::getValue() {

	GLfloat *result = new GLfloat[4];
	
	for(int i=0; i<4; i++)
		result[i] = _quaternion[i];

	return result;
}

void Quaternion::setValue(const GLfloat value[4]) {

	for(int i=0; i<4; i++)
		_quaternion[i] = value[i];
}

void Quaternion::lerp(Quaternion quaternion, GLfloat k) {	

	GLfloat angle =	  _quaternion[QT] * quaternion[QT] 
					+ _quaternion[QX] * quaternion[QX]
					+ _quaternion[QY] * quaternion[QY]
					+ _quaternion[QZ] * quaternion[QZ];

	GLfloat k0 = 1.0f - k;
	GLfloat k1 = (angle > 0) ? k : -k;

	Quaternion temporary(quaternion.getValue());

	(*this) *= k0;
	temporary *= k1;

	(*this) += temporary;
}

void Quaternion::slerp(Quaternion quaternion, GLfloat k) {

	GLfloat angle =	acos(     _quaternion[QT] * quaternion[QT] 
							+ _quaternion[QX] * quaternion[QX]
							+ _quaternion[QY] * quaternion[QY]
							+ _quaternion[QZ] * quaternion[QZ]);

	GLfloat k0 = sin((1-k)*angle) / sin(angle);
	GLfloat k1 = sin(k*angle) / sin(angle);

	Quaternion temporary(quaternion.getValue());

	(*this) *= k0;
	temporary *= k1;

	(*this) += temporary;
}

GLfloat& Quaternion::operator [] (int position) {

	return _quaternion[position];
}

Quaternion Quaternion::operator + (Quaternion quaternion) {

	Quaternion result;

	for(int i=0; i<4; i++)
		result[i] = _quaternion[i] + quaternion[i];

	return result;
}

Quaternion Quaternion::operator += (Quaternion quaternion) {

	for(int i=0; i<4; i++)
		_quaternion[i] += quaternion[i];

	return (*this);
}

Quaternion Quaternion::operator * (Quaternion quaternion) {

	Quaternion result;

	Quaternion temporary(getValue());

	result[QT] =	  temporary[QT] * quaternion[QT] 
					- temporary[QX] * quaternion[QX]
					- temporary[QY] * quaternion[QY]
					- temporary[QZ] * quaternion[QZ];

	result[QX] =	  temporary[QT] * quaternion[QX]
					+ temporary[QX] * quaternion[QT]
					+ temporary[QY] * quaternion[QZ]
					- temporary[QZ] * quaternion[QY];

	result[QY] =	  temporary[QT] * quaternion[QY]
					+ temporary[QY] * quaternion[QT]
					+ temporary[QZ] * quaternion[QX]
					- temporary[QX] * quaternion[QZ];

	result[QZ] =	  temporary[QT] * quaternion[QZ]
					+ temporary[QZ] * quaternion[QT]
					+ temporary[QX] * quaternion[QY]
					- temporary[QY] * quaternion[QX];
	return result;
}

Quaternion Quaternion::operator *= (Quaternion quaternion) {

	Quaternion temporary(getValue());

	_quaternion[QT] =	  temporary[QT] * quaternion[QT] 
						- temporary[QX] * quaternion[QX]
						- temporary[QY] * quaternion[QY]
						- temporary[QZ] * quaternion[QZ];

	_quaternion[QX] =	  temporary[QT] * quaternion[QX]
						+ temporary[QX] * quaternion[QT]
						+ temporary[QY] * quaternion[QZ]
						- temporary[QZ] * quaternion[QY];

	_quaternion[QY] =	  temporary[QT] * quaternion[QY]
						+ temporary[QY] * quaternion[QT]
						+ temporary[QZ] * quaternion[QX]
						- temporary[QX] * quaternion[QZ];

	_quaternion[QZ] =	  temporary[QT] * quaternion[QZ]
						+ temporary[QZ] * quaternion[QT]
						+ temporary[QX] * quaternion[QY]
						- temporary[QY] * quaternion[QX];
	return (*this);
}

Quaternion Quaternion::operator * (GLfloat scalar) {

	Quaternion result;

	for(int i=0; i<4; i++)
		result[i] = _quaternion[i] * scalar;

	return result;
}

Quaternion Quaternion::operator *= (GLfloat scalar) {

	for(int i=0; i<4; i++)
		_quaternion[i] *= scalar;

	return (*this);
}

bool Quaternion::operator == (Quaternion quaternion) {

	for(int i=0; i<4; i++)
		if(!(fabs(_quaternion[i] - quaternion[i]) < threshold)) 
			return false;

	return true;
}

void Quaternion::dump() {

	cout << "<Quaternion Dump> = ";

	cout.precision(5);

	for(int i=0; i<4; i++)
		cout << " [" << _quaternion[i] << "]";

	cout << endl;
}
