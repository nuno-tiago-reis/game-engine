#include "Camera.h"

Camera::Camera(string name) {

	_name = name;

	_viewMatrix.loadIdentity();
	_projectionMatrix.loadIdentity();

	_projectionMode = ORTHOGONAL;

	_uniformBufferIndex = UINT_MAX;

	_width = WIDTH;
	_height = HEIGHT;

	_zoom = 1.0f;
	_longitude = 0.0f;
	_latitude = 89.0f;

	loadView();
	loadOrthogonalProjection();
}

Camera::~Camera() {
}

void Camera::update(GLint zoom, GLint longitude, GLint latitude, GLfloat elapsedTime) {

	if(longitude == 0 && latitude == 0 && zoom == 0)
		return;

	/* Update the Zoom */
	_zoom -= zoom * 0.05f;

	if(_zoom < 0.1f)
		_zoom = 0.1f;
	
	/* Update the Longitude */
	_longitude += longitude * elapsedTime * 5.0f;

	if(_longitude > 360.0f)
		_longitude -= 360.0f;
	else if(_longitude < -360.0f)
		_longitude += 360.0f;

	/* Update the Latitude */
	_latitude += latitude * elapsedTime * 5.0f;

	if(_latitude > 360.0f)
		_latitude -= 360.0f;
	else if(_latitude < -360.0f) 
		_latitude += 360.0f;

	/* Update the Shader Uniforms */
	loadView();
	loadUniforms();
}

void Camera::reshape(GLint width, GLint height) {

	_width = width;
	_height = height;

	if(_projectionMode == PERSPECTIVE)
		loadPerspectiveProjection();
}

void Camera::loadUniforms() {

	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndex);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat[16]), _viewMatrix.getValue());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat[16]), sizeof(GLfloat[16]), _projectionMatrix.getValue());

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::loadView() {

	_eye[VX] = _position[VX] + _zoom * CAMERA_RADIUS * sin((_latitude - 90.0f) * DEGREES_TO_RADIANS) * cos(_longitude * DEGREES_TO_RADIANS);
	_eye[VY] = _position[VY] + _zoom * CAMERA_RADIUS * sin((_latitude - 90.0f) * DEGREES_TO_RADIANS) * sin(_longitude * DEGREES_TO_RADIANS);
	_eye[VZ] = _position[VZ] + _zoom * CAMERA_RADIUS * cos((_latitude - 90.0f) * DEGREES_TO_RADIANS);
	_eye[VW] = 1.0f;

	_target[VX] = _position[VX];
	_target[VY] = _position[VY];
	_target[VZ] = _position[VZ];
	_target[VW] = 1.0f;

	_up[VX] = 0.0f;
	_up[VY] = 0.0f;
	_up[VZ] = cos(_latitude * DEGREES_TO_RADIANS);
	_up[VW] = 1.0f;
	
	_viewMatrix.loadIdentity();
	_viewMatrix.setView(_eye,_target,_up);
	_viewMatrix.transpose();
}

void Camera::loadOrthogonalProjection() {

	_projectionMode = ORTHOGONAL;

	_projectionMatrix.loadIdentity();
	_projectionMatrix.setOrthogonalProjection(ORTHO_LEFT,ORTHO_RIGHT,ORTHO_TOP,ORTHO_BOTTOM,ORTHO_NEAR,ORTHO_FAR);
	_projectionMatrix.transpose();
}

void Camera::loadOrthogonalProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ, GLfloat farZ) {

	_projectionMode = ORTHOGONAL;

	_projectionMatrix.loadIdentity();
	_projectionMatrix.setOrthogonalProjection(left,right,top,bottom,nearZ,farZ);
	_projectionMatrix.transpose();
}

void Camera::loadPerspectiveProjection() {

	_projectionMode = PERSPECTIVE;

	_projectionMatrix.loadIdentity();
	_projectionMatrix.setPerspectiveProjection(PERS_ANGLE,(GLfloat)_width/_height,PERS_NEAR,PERS_FAR);
	_projectionMatrix.transpose();
}

void Camera::loadPerspectiveProjection(GLfloat angle, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ) {

	_projectionMode = PERSPECTIVE;

	_projectionMatrix.loadIdentity();
	_projectionMatrix.setPerspectiveProjection(angle,aspectRatio,nearZ,farZ);
	_projectionMatrix.transpose();
}

string Camera::getName() {

	return _name;
}

Matrix Camera::getViewMatrix() {

	return _viewMatrix;
}
Matrix Camera::getProjectionMatrix() {

	return _projectionMatrix;
}

GLint Camera::getProjectionMode() {

	return _projectionMode;
}

GLuint Camera::getUniformBufferIndex() {

	return _uniformBufferIndex;
}

GLint Camera::getWidth() {

	return _width;
}

GLint Camera::getHeight() {

	return _height;
}

GLfloat Camera::getZoom() {

	return _zoom;
}

GLfloat Camera::getLongitude() {

	return _longitude;
}

GLfloat Camera::getLatitude() {
	
	return _latitude;
}

Vector Camera::getPosition() {

	return _position;
}

Vector Camera::getTarget() {

	return _target;
}

Vector Camera::getEye() {

	return _eye;
}

Vector Camera::getUp() {

	return _up;
}

void Camera::setName(string name) {

	_name = name;
}
		
void Camera::setViewMatrix(Matrix viewMatrix) {

	_viewMatrix.setValue(viewMatrix.getValue());
}

void Camera::setProjectionMatrix(Matrix projectionMatrix) {

	_projectionMatrix.setValue(projectionMatrix.getValue());
}

void Camera::setProjectionMode(GLint projectionMode) {

	_projectionMode = projectionMode;
}

void Camera::setUniformBufferIndex(GLuint uniformBufferIndex) {

	_uniformBufferIndex = uniformBufferIndex;
}

void Camera::setWidth(GLint width) {
	
	_width = width;
}

void Camera::setHeight(GLint height) {

	_height = height;
}

void Camera::setZoom(GLfloat zoom) {

	_zoom = zoom;
}

void Camera::setLongitude(GLfloat longitude) {

	_longitude = longitude;
}

void Camera::setLatitude(GLfloat latitude) {

	_latitude = latitude;
}

void Camera::setPosition(Vector position) {

	_position = position;
}

void Camera::setTarget(Vector target) {

	_target.setValue(target.getValue());
}

void Camera::setEye(Vector eye) {

	_eye.setValue(eye.getValue());
}

void Camera::setUp(Vector up) {

	_up.setValue(up.getValue());
}

void Camera::dump() {

	cout << "<Camera Dump>" << endl;

	/* Camera View Matrix */
	cout << "<Camera View Matrix> = " << endl;
	_viewMatrix.dump();

	/* Camera View Matrix */
	cout << "<Camera Projection Matrix> = " << endl;
	_projectionMatrix.dump();

	/* Camera Projection Mode (Perspective or Orthogonal)*/
	if(_projectionMode == ORTHOGONAL)
		cout << "<Camera Projection Mode> = " << "ORTHOGONAL ;" << endl;
	else
		cout << "<Camera Projection Mode> = " << "PERSPECTIVE ;" << endl;

	/* Viewport Width & Height */
	cout << "<Camera Viewport Width> = " << _width << " ;" << endl;
	cout << "<Camera Viewport Height> = " << _height << " ;" << endl;

	/* Camera Zoom */
	cout << "<Camera Zoom> = " << _zoom << " ;" << endl;

	/* Camera Spherical Coordinates */
	cout << "<Camera Longitude> = " << _longitude << " ;" << endl;
	cout << "<Camera Latitude> = " << _latitude << " ;" << endl;

	/* Camera Position */
	cout << "<Camera Position> = ";
	_position.dump();
	
	/* Camera LookAt Vectors */
	cout << "<Camera Eye> = "; 
	_eye.dump();
	cout << "<Camera Target> = ";
	_target.dump();
	cout << "<Camera Up> = ";
	_up.dump();
}