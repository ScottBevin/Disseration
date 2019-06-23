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
#include <string>

#include "CEGUI/CEGUIWindowManager.h"
#include <CEGUI/CEGUI.h>

#pragma once

class DebugCEGUI
{
private:
	bool mInverted;
	bool mEnabled;
	int mKeepThreshold;
		
	CEGUI::WindowManager *mWindowManager;
	
	
	//static DebugCEGUI* quickInstance;
public:
	CEGUI::Listbox* mDebugBox; 
	void Log(std::string message);

	void OnMessage(std::string message);

	//static DebugCEGUI& GetQuickInstance(){return *quickInstance;}
	
	void DisableDebug(){mWindowManager->getWindow("Debug")->setEnabled(false);};
	void EnableDebug(){mWindowManager->getWindow("Debug")->setEnabled(true);};
	void HideDebug(){mWindowManager->getWindow("Debug")->setVisible(false);};
	void ShowDebug(){mWindowManager->getWindow("Debug")->setVisible(true);};
	void Initialize(std::string skin,  std::string WindowTag = "Root");
		
	/*How many items will the debug store in the debug listbox (please note that the app
	will slow down at high numbers)*/
	void SetKeepThreshold(int number){mKeepThreshold=number;}

		DebugCEGUI() : mInverted(false) , mEnabled(true) , mKeepThreshold(300){};

		~DebugCEGUI();
};


