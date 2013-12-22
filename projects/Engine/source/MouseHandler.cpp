#include "MouseHandler.h"

MouseHandler* MouseHandler::instance = NULL;

MouseHandler::MouseHandler() {

	_mouseEnabled = false;

	_mouseWheelPosition = 0;
}

MouseHandler::~MouseHandler() {

}

MouseHandler* MouseHandler::getInstance() {

	if(instance == NULL)
		instance = new MouseHandler();

	return instance;
}

void MouseHandler::destroyInstance() {

	delete instance;

	instance = NULL;
}

void MouseHandler::enableMouse() {

	_previousButtonMap.clear();
	_previousButtonMap.insert(_currentButtonMap.begin(),_currentButtonMap.end());

	_mouseEnabled = true;
}

void MouseHandler::disableMouse() {

	_mouseEnabled = false;
}

void MouseHandler::mouseMovementListener(int x, int y) {

	if(!_mouseEnabled)
		return;

	if(_currentButtonMap[GLUT_LEFT_BUTTON] == true) {
		
		_currentLongitudeMap[GLUT_LEFT_BUTTON] = x;
		_currentLatitudeMap[GLUT_LEFT_BUTTON] = y;
	}

	if(_currentButtonMap[GLUT_RIGHT_BUTTON] == true) {
		
		_currentLongitudeMap[GLUT_RIGHT_BUTTON] = x;
		_currentLatitudeMap[GLUT_RIGHT_BUTTON] = y;
	}

	_currentLongitudeMap[DEFAULT] = x;
	_currentLatitudeMap[DEFAULT] = y;
}

void MouseHandler::mousePassiveMovementListener(int x, int y) {

	if(!_mouseEnabled)
		return;

	_currentLongitudeMap[DEFAULT] = x;
	_currentLatitudeMap[DEFAULT] = y;
}

void MouseHandler::mouseEventListener(int button, int state, int x, int y) {  

	if(!_mouseEnabled)
		return;

	if(state == GLUT_UP)
		_currentButtonMap[button] = false;
	else if(state == GLUT_DOWN)
		_currentButtonMap[button] = true;

	_currentLongitudeMap[button] = _oldLongitudeMap[button] = x;
	_currentLatitudeMap[button] = _oldLatitudeMap[button] = y;

	_currentLongitudeMap[DEFAULT] = x;
	_currentLatitudeMap[DEFAULT] = y;
}

void MouseHandler::mouseWheelListener(int button, int direction, int x, int y) {

	if(!_mouseEnabled)
		return;

	if(direction > 0)
		_mouseWheelPosition++;
	else
		_mouseWheelPosition--;
}

bool MouseHandler::isButtonPressed(int button) {

	return _currentButtonMap[button];
}

bool MouseHandler::wasButtonPressed(int button) {

	return _previousButtonMap[button];
}

int MouseHandler::getLongitude(int button) {

	GLint longitude = _currentLongitudeMap[button] - _oldLongitudeMap[button];

	_oldLongitudeMap[button] = _currentLongitudeMap[button];

	return longitude;
}

int MouseHandler::getLatitude(int button) {

	GLint latitude =  _currentLatitudeMap[button] - _oldLatitudeMap[button];

	_oldLatitudeMap[button] = _currentLatitudeMap[button];

	return latitude;
}

int MouseHandler::getMouseWheelPosition() { 

	GLint position = _mouseWheelPosition;

	_mouseWheelPosition = 0;

	return position;
}

int* MouseHandler::getMouseClickPosition(int button) {

	GLint *position = new int[2];

	position[0] = _currentLongitudeMap[button];
	position[1] = _currentLatitudeMap[button];

	return position;
}

bool MouseHandler::isOutsideViewport() {

	//cout << "Longitude: " << _currentLongitudeMap[DEFAULT] / 640 - 1.0f << endl;
	//cout << "Latitude: " << 1.0f - (2.0f * _currentLatitudeMap[DEFAULT]) / 640 << endl;

	//cout << _currentLongitudeMap[GLUT_LEFT_BUTTON]  << _currentLatitudeMap[GLUT_LEFT_BUTTON] << endl;
	//cout << _currentLongitudeMap[GLUT_RIGHT_BUTTON]  << _currentLatitudeMap[GLUT_RIGHT_BUTTON] << endl;
	//cout << _currentLongitudeMap[DEFAULT]  << _currentLatitudeMap[DEFAULT] << endl;

	return false;
}

bool MouseHandler::isMouseEnabled() {

	return _mouseEnabled;
}