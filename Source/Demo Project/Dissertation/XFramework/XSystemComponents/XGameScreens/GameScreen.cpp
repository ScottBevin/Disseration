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

#include "GameScreen.h"

namespace XF
{
	GameScreen::GameScreen(bool isPopup)
	{
		mTransitionOnTime = 0.5f;
		mTransitionOffTime = 0.5f;
		mIsPopup = isPopup;
	}

	GameScreen::~GameScreen()
	{
	}

	void GameScreen::Initialise(GameScreenManagerComponent *screenManager)
	{
		mScreenManager = screenManager;

		mRenderer = mScreenManager->GetRenderer();
		
		TransitionOn();

		OnInitialise();
		
		mScreenLifeTime = 0.0f;
	}

	void GameScreen::HandleInput(double dT, Input *input)
	{
		OnHandleInput(dT, input);
	}

	void GameScreen::Update(double dT)
	{
		switch(mScreenState)
		{
		case ScreenState::SCREEN_TRANSITION_ON:
			if((mTransitionTimer += dT) >= mTransitionOnTime)
			{
				OnScreenJustWentActive();
				mScreenState = ScreenState::SCREEN_ACTIVE;
			}
			break;
		case ScreenState::SCREEN_ACTIVE:
			mScreenLifeTime += dT;
			break;
		case ScreenState::SCREEN_TRANSITION_OFF:
			if((mTransitionTimer += dT) >= mTransitionOffTime)
			{
				OnScreenJustWentInActive();
				mScreenState = ScreenState::SCREEN_NOT_ACTIVE;
			}
			break;
		}

		OnUpdate(dT);
	}

	void GameScreen::Shutdown()
	{
		OnShutDown();
	}

	void GameScreen::TransitionOn()
	{
		mScreenState = ScreenState::SCREEN_TRANSITION_ON;
		mTransitionTimer = 0.0f;
	}

	void GameScreen::TransitionOff()
	{
		mScreenState = ScreenState::SCREEN_TRANSITION_OFF;
		mTransitionTimer = 0.0f;
	}

	float GameScreen::TransitionDelta()
	{
		switch(mScreenState)
		{
		case ScreenState::SCREEN_TRANSITION_ON:
			return (mTransitionTimer / mTransitionOnTime);
		case ScreenState::SCREEN_ACTIVE:
			return 1.0f;
		case ScreenState::SCREEN_TRANSITION_OFF:
			return (1.0f - (mTransitionTimer / mTransitionOffTime));
		}

		return 0;
	}
}
