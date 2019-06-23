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

#include <map>
#include <CEGUI\CEGUI.h>
#include <string>

#include "..\SystemComponent.h"
#include "CEGuiDebugVariableTrackerWindow.h"
#include "..\XCEGui\CEGUIComponent.h"
#include "CEGuiDebuggerTool.h"
#include "XMessaging\Messenger.h"

namespace XF
{
	class CEGuiDebuggingTools :
		public SystemComponent,
		public Messenger
	{
	private:
		CEGUIComponent* mGui;
		CEGUI::FrameWindow *mWindow;
		CEGUI::Window* mDebugRoot;

		std::string mSkinFile;
		const float mButtonWidth, mButtonHeight, mSpacing;

		typedef std::map<std::string, std::pair<CEGUI::ButtonBase*, CEGuiDebuggerTool*> > ToolsMap;
		typedef std::map<std::string, std::pair<CEGUI::ButtonBase*, CEGuiDebuggerTool*> >::iterator ToolsMapIt;
		ToolsMap mTools;

		void CreateDebugWindow();
		void RejigPositioning();

		bool ButtonClicked( const CEGUI::EventArgs &args );
		bool TitlebarDoubleClicked( const CEGUI::EventArgs &args );

	public:
		CEGuiDebuggingTools(Application* ownerApp, std::string skinFile);
		~CEGuiDebuggingTools();

		void OnInitialise();
		void OnShutDown();
		bool OnHandleMessage(const Message &message);

		void RegisterTool(std::string name, std::string buttonText, CEGuiDebuggerTool* tool);
		void DestroyTool(std::string name);
	};
}
