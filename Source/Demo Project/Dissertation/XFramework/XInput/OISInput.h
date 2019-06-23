/*************************************
Author: Christopher Pepper 12/2/2010
***************************************/
#pragma once
#include "Input.h"
#include <OIS/OIS.h>
#include <sstream>

class OISInput :
	public XF::Input
{
private:
	OIS::Keyboard *mOISKeyboard;
	OIS::Mouse *mOISMouse;
	OIS::InputManager *mInput;
	
public:

	void OnInitialize();
	void OnUpdate();


	OISInput(size_t windowHnd);
	~OISInput();
};
