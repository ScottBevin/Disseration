/*************************************
Author: Christopher Pepper 12/2/2010
***************************************/
#include "OISInput.h"
#include <iostream>

OISInput::OISInput(size_t windowHnd)
{
	std::ostringstream windowHndStr;
	OIS::ParamList pl;

	windowHndStr << windowHnd;
	pl.insert( std::make_pair( std::string("WINDOW"), windowHndStr.str()  ) );


#if defined OIS_WIN32_PLATFORM
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif




	mInput = OIS::InputManager::createInputSystem(pl);


	mOISKeyboard = static_cast<OIS::Keyboard*>(mInput->createInputObject(OIS::OISKeyboard, false));
	mOISMouse = static_cast<OIS::Mouse*>(mInput->createInputObject(OIS::OISMouse, false));

	mMouse.X = 0;
	mMouse.Y = 0;

	mMouse.OldScrollPos = mMouse.ScrollPos = 0;
}

OISInput::~OISInput()
{
	OIS::InputManager::destroyInputSystem(mInput);
}
void OISInput::OnInitialize()
{	
	 
}
void OISInput::OnUpdate()
{
	char keys[NUM_KEYS];

	mOISKeyboard->capture();
	mOISKeyboard->copyKeyStates(keys);
	mMouse.OldScrollPos = mMouse.ScrollPos;

	for(int i = 0; i < NUM_KEYS; ++i)
	{
		mKeyBoard.mCurrKeys[i] = (bool)keys[i];
	}

	mOISMouse->capture();
	//std::cout <<  mOISMouse->getMouseState().X.rel;
	mMouse.X += mOISMouse->getMouseState().X.rel;
	mMouse.Y -= mOISMouse->getMouseState().Y.rel;
	mMouse.ScrollPos = (float)mOISMouse->getMouseState().Z.abs;
	
	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		mMouse.Buttons[i] = mOISMouse->getMouseState().buttonDown((OIS::MouseButtonID)i);
	}
}
