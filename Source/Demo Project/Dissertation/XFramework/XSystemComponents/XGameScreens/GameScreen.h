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

#pragma once

#include "..\..\XInput\Input.h"
#include "GameScreenManagerComponent.h"
#include "..\XOgre\OgreRenderer.h"
#include "XMessaging\Messenger.h"

namespace XF
{
	namespace ScreenState
	{
		enum ENUM
		{
			SCREEN_TRANSITION_ON,
			SCREEN_ACTIVE,
			SCREEN_TRANSITION_OFF,
			SCREEN_NOT_ACTIVE,
		};
	}

	class GameScreen : public Messenger
	{
	private:
		bool mKillScreen;
		bool mIsPopup;

		ScreenState::ENUM mScreenState;
		float mScreenLifeTime, mTransitionTimer;

	protected:
		virtual void OnInitialise() = 0;
		virtual void OnHandleInput(double dT, Input *input) = 0;
		virtual void OnUpdate(double dT) = 0;
		virtual void OnShutDown() = 0;
		
		virtual void OnScreenJustWentActive(){}
		virtual void OnScreenJustWentInActive(){}

		virtual void OnShowScreen(){}
		virtual void OnHideScreen(){}

		float mTransitionOnTime, mTransitionOffTime;
		OgreRenderer *mRenderer;
		GameScreenManagerComponent *mScreenManager;

	public:
		GameScreen(bool isPopup);
		virtual ~GameScreen();

		OgreRenderer *GetRenderer(){return mRenderer;}
		void Initialise(GameScreenManagerComponent *screenManager);
		void HandleInput(double dT, Input *input);
		void Update(double dT);
		void Shutdown();

		void ExitScreen(){ mKillScreen = true; TransitionOff();}
		bool ScreenWantsToExit(){ return mKillScreen; }

		bool IsPopup(){ return mIsPopup; }

		ScreenState::ENUM GetScreenState(){return mScreenState;}

		void TransitionOn();
		void TransitionOff();

		float TransitionDelta();
		float ScreenLifetime(){return mScreenLifeTime;}

		GameScreenManagerComponent* ScreenManager(){return mScreenManager;}
	};
}
