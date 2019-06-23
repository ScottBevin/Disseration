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

#include "GameScreenManagerComponent.h"

#include "GameScreen.h"
#include "../../XUtilities/Logger.h"

namespace XF
{
	GameScreenManagerComponent::GameScreenManagerComponent(Application *ownerApp) : 
		SystemComponent(ownerApp)
	{
	}

	GameScreenManagerComponent::~GameScreenManagerComponent()
	{
	}

	void GameScreenManagerComponent::OnInitialise()
	{
		for(unsigned int i = 0; i < mGameScreens.size(); ++i)
		{
			mGameScreens[i]->Initialise(this);
		}
	}

	void GameScreenManagerComponent::OnHandleInput(double dT, Input *input)
	{
		if(!mGameScreens.empty())
		{
			GameScreen *backScreen = mGameScreens.back();

			if(backScreen->GetScreenState() != ScreenState::SCREEN_NOT_ACTIVE)
				backScreen->HandleInput(dT, input);
		}
	}

	void GameScreenManagerComponent::OnUpdate(double dT)
	{
		GameScreen* oldTopScreen = mGameScreens[mGameScreens.size() - 1];

		for(unsigned int i = 0; i < mGameScreens.size(); ++i)
		{
			GameScreen* thisScreen = mGameScreens[i];

			if(thisScreen->ScreenWantsToExit() && thisScreen->GetScreenState() == ScreenState::SCREEN_NOT_ACTIVE)
			{
				thisScreen->Shutdown();

				mGameScreens.erase(mGameScreens.begin() + i);

				delete thisScreen;
			}
			else
			{
				thisScreen->Update(dT);
			}
		}

		if(oldTopScreen != mGameScreens[mGameScreens.size() - 1])
		{
			if(mGameScreens[mGameScreens.size() - 1]->GetScreenState() == ScreenState::SCREEN_NOT_ACTIVE)
			{
				int i = (int)mGameScreens.size();

				do 
				{
					--i;
					mGameScreens[i]->TransitionOn();
				} while (mGameScreens[i]->IsPopup() && i >= 0);
			}
		}
	}

	void GameScreenManagerComponent::OnShutDown()
	{
		for(unsigned int i = 0; i < mGameScreens.size(); ++i)
		{
			mGameScreens[i]->Shutdown();
			delete mGameScreens[i];
		}
	}

	void GameScreenManagerComponent::AddNewScreen(GameScreen* screen)
	{
		std::vector<GameScreen*>::iterator it = std::find(
			mGameScreens.begin(),
			mGameScreens.end(),
			screen);

		if(it == mGameScreens.end())
		{
			if(Initialised())
			{
				screen->Initialise(this);
			}
 
 			if(!screen->IsPopup())
 			{
 				for(unsigned int i = 0; i < mGameScreens.size(); ++i)
 				{
 					GameScreen* thisScreen = mGameScreens[i];
 
 					thisScreen->TransitionOff();
 				}
 			}

			mGameScreens.push_back(screen);
		}
		else
		{
			ILOG << "[GameScreenManagerComponent::AddNewScreen]: Attempted to add same screen instance more then once";
		}
	}

	void GameScreenManagerComponent::AddNewScreenUnderneathCurrentScreen( GameScreen* newScreen, GameScreen* currentScreen )
	{
		int idx = -1;

		for(unsigned int i = 0; i < mGameScreens.size(); ++i)
		{
			if(mGameScreens[i] == currentScreen)
			{
				idx = i;
			}

			if(mGameScreens[i] == newScreen)
			{
				ILOG << "[GameScreenManagerComponent::AddNewScreenUnderneathCurrentScreen]: Attempted to add same screen instance more then once";
			}
		}

		if(idx != -1)
		{
			mGameScreens.insert(mGameScreens.begin() + idx, newScreen);
		}
		else
		{
			ILOG << "[GameScreenManagerComponent::AddNewScreenUnderneathCurrentScreen]: Attempted to add screen under screent hat doesnt exist";
		}
	}

	void GameScreenManagerComponent::KillAllScreens()
	{
		for(unsigned int i = 0; i < mGameScreens.size(); ++i)
		{
			mGameScreens[i]->ExitScreen();
		}
	}
}
