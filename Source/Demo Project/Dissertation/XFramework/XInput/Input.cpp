/**************************************************************************************************
 *
 *	XCS Crowd Simulation Software
 *	Author: Scott Bevin
 *	Web: www.gameXcore.co.uk
 *	Email: scott@gameXcore.co.uk
 *	All code (c)2010 Scott Bevin, all rights reserved.
 *	
 *	This software is part of the disssertation work entitled "Real time crowd simulations incorporating individual
 *	agent personality models and group interactions" submitted to The University of Bolton by Scott Bevin in partial
 *	fulfilment of the requirements of The University of Bolton for the degree of Bachelor of Science in May 2010.
 *	
 **************************************************************************************************/

 /************************************************************************

 This file is part of "Real time crowd simulations incorporating individual agent personality models and group interactions".

 "Real time crowd simulations incorporating individual agent personality 
 models and group interactions" is free software: you can redistribute 
 it and/or modify it under the terms of the GNU General Public License 
 as published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 "Real time crowd simulations incorporating individual agent personality models and group interactions" is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with "Real time crowd simulations incorporating individual agent personality models and group interactions".  
 If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

// All code (c)2010 Scott Bevin, all rights reserved.

#include "Input.h"
/*most of the stuff in here is to help my dissertation project
by faking key presses and mouse downs so that ordinary mouse and key board
already being used in an app can be switched straight to wii controls*/
namespace XF
{
	void Input::FakeKeyPress( int key )
	{
		mKeyBoard.mCurrKeys[key] = true;
		mKeyBoard.mOldKeys[key]  =false;
	}

	void Input::FakeKeyDown( int key )
	{
		mKeyBoard.mCurrKeys[key] = mKeyBoard.mOldKeys[key]  =true;
	}

	void Input::FakeKeyUp( int key )
	{
		mKeyBoard.mCurrKeys[key] = mKeyBoard.mOldKeys[key]  =false;
	}

	void Input::FakeKeyRelease( int key )
	{
		mKeyBoard.mCurrKeys[key] = false;
		mKeyBoard.mOldKeys[key]  =true;
	}

	void Input::FakeMouseButtonPress( int button )
	{
		mMouse.Buttons[button] = true;
		mMouse.OldButtons[button] = false;
	}

	void Input::FakeMouseButtonDown( int button )
	{
		mMouse.Buttons[button] = mMouse.OldButtons[button]  =true;
	}

	void Input::FakeMouseButtonUp( int button )
	{
		mMouse.Buttons[button] = mMouse.OldButtons[button]  =false;
	}		

	void Input::FakeMouseButtonRelease( int button )
	{
		mMouse.Buttons[button] = false;
		mMouse.OldButtons[button] = true;
	}

	void Input::SetMouseX(int x)
	{
		SetCursorPos(x,GetMouseY());	
	}

	void Input::SetMouseY(int y)
	{
		SetCursorPos(GetMouseX(),y);
	}
	void Input::SetMouseCursor(int x, int y)
	{
		SetCursorPos(x,y);
	}

	bool Input::IsMouseMoving( DWORD movementType  )
	{
		if((movementType & MovingTypes::mvLeft) == MovingTypes::mvLeft)
		{
			if(mMouse.X < mMouse.OldX)
			{
				return true;
			}
		}
		if((movementType & MovingTypes::mvRight) == MovingTypes::mvRight)
		{
			if(mMouse.X > mMouse.OldX)
			{
				return true;
			}
		}
		if((movementType & MovingTypes::mvDown) == MovingTypes::mvDown)
		{
			if(mMouse.Y < mMouse.OldY)return true;
		}
		if((movementType & MovingTypes::mvUp) == MovingTypes::mvUp)
		{
			if(mMouse.Y > mMouse.OldY)return true;
		}
		return false;
	}


	void Input::Update()
	{	
		//copy current keys into old keys
		memcpy(mKeyBoard.mOldKeys, mKeyBoard.mCurrKeys, sizeof(bool)*NUM_KEYS);
		//copy current keys into old keys
		memcpy(mMouse.OldButtons, mMouse.Buttons, sizeof(bool)*3);

		mMouse.OldX = mMouse.X;
		mMouse.OldY = mMouse.Y;

		OnUpdate();
	}
}