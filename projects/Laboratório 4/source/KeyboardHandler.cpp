#include "KeyboardHandler.h"

KeyboardHandler* KeyboardHandler::instance = NULL;

KeyboardHandler::KeyboardHandler() {

	_keyboardEnabled = false;
}

KeyboardHandler::~KeyboardHandler() {

}

KeyboardHandler* KeyboardHandler::getInstance() {

	if(instance == NULL)
		instance = new KeyboardHandler();

	return instance;
}

void KeyboardHandler::destroyInstance() {

	delete instance;

	instance = NULL;
}

void KeyboardHandler::enableKeyboard() {

	_previousKeyMap.clear();
	_previousKeyMap.insert(_currentKeyMap.begin(),_currentKeyMap.end());

	_previousSpecialKeyMap.clear();
	_previousSpecialKeyMap.insert(_currentSpecialKeyMap.begin(),_currentSpecialKeyMap.end());

	_keyboardEnabled = true;
}

void KeyboardHandler::disableKeyboard() {

	_keyboardEnabled = false;
}

void KeyboardHandler::releasedNormalKeyListener(unsigned char key, int x, int y) {  

	if(!_keyboardEnabled)
		return;

	_currentKeyMap[key] = false;
}

void KeyboardHandler::releasedSpecialKeyListener(int key, int x, int y) {  
	
	if(!_keyboardEnabled)
		return;

	_currentSpecialKeyMap[key] = false;
}

void KeyboardHandler::normalKeyListener(unsigned char key, int x, int y) {  

	if(!_keyboardEnabled)
		return;

	if(_currentKeyMap[key] == true)
		_previousKeyMap[key] = true;

	_currentKeyMap[key] = true;
}  

void KeyboardHandler::specialKeyListener(int key, int x, int y) {  

	if(!_keyboardEnabled)
		return;

	if(_currentSpecialKeyMap[key] == true)
		_previousSpecialKeyMap[key] = true;

	_currentSpecialKeyMap[key] = true;
}  

bool KeyboardHandler::isKeyPressed(unsigned char key) {

	return _currentKeyMap[key];
}

bool KeyboardHandler::wasKeyPressed(unsigned char key) {

	return _previousKeyMap[key];
}

bool KeyboardHandler::wasKeyPressedThisFrame(unsigned char key){
	return(isKeyPressed(key) && !wasKeyPressed(key));	
}
		
bool KeyboardHandler::isSpecialKeyPressed(int key) {

	return _currentSpecialKeyMap[key];
}

bool KeyboardHandler::wasSpecialKeyPressed(int key) {

	return _previousSpecialKeyMap[key];
}

bool KeyboardHandler::wasSpecialKeyPressedThisFrame(int key){
	return(isSpecialKeyPressed(key) && !wasSpecialKeyPressed(key));
}

bool KeyboardHandler::isKeyboardEnabled() {

	return _keyboardEnabled;
}