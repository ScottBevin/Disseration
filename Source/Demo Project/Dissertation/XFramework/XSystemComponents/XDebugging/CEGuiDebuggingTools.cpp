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


#include "CEGuiDebuggingTools.h"
#include "CEGuiDebugMessageLogger.h"
#include "CEGUIFunctionRunner.h"

#include "XUtilities\Logger.h"

namespace XF
{
	CEGuiDebuggingTools::CEGuiDebuggingTools(Application* ownerApp, std::string skinFile)
		: SystemComponent(ownerApp), mSkinFile(skinFile), mButtonWidth(100), mButtonHeight(20), mSpacing(5)
	{
	}

	CEGuiDebuggingTools::~CEGuiDebuggingTools()
	{
	}

	void CEGuiDebuggingTools::OnInitialise()
	{
		PostAMessage(
			XF::MailingList::GUI_SUBSYSTEM,
			XF::MessageSubject::PLEASE_SEND_GUI,
			NULL);	
		
		mDebugRoot = mGui->CreateBlankSheet("DebugToolsRoot");


		CreateDebugWindow();

		RegisterTool("VariableTrackerWindow", "Variables", new CEGuiDebugVariableTrackerWindow());
		RegisterTool("MessageLoggerWindow", "Logger", new CEGuiDebugMessageLogger());
		RegisterTool("RunAFunction", "Run", new CEGUIFunctionRunner());
	}

	void CEGuiDebuggingTools::OnShutDown()
	{
		for(ToolsMapIt it = mTools.begin();
			it != mTools.end();)
		{
			std::string toolName = it->first;

			++it;

			DestroyTool(toolName);
		}

		mGui->KillWindow(mDebugRoot);
	}

	bool CEGuiDebuggingTools::OnHandleMessage( const Message &message )
	{
		switch(message.Subject)
		{
		case MessageSubject::SENT_GUI:
			{
				mGui = static_cast<CEGUIComponent*>(message.Data);
				return true;
			}
		}
		return false;
	}

	void CEGuiDebuggingTools::CreateDebugWindow()
	{
		mWindow = mGui->CreateFrameWindow("CEGuiDebuggingToolsWindow");//chris: updated this as a test, works fine
		//(CEGUI::FrameWindow*)CEGUI::WindowManager::getSingletonPtr()->createWindow(mSkinFile + "/FrameWindow", "CEGuiDebuggingToolsWindow");
		mWindow->setText("XFramework Debugging Tools");
		mWindow->setVisible(true);
		mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
		mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0, 40)));
		mWindow->setAlpha(0.5f);
		mWindow->setSizingEnabled(false);
		mWindow->setDragMovingEnabled(true);
		//mWindow->setAlwaysOnTop(true);
		mWindow->setCloseButtonEnabled(false);

		mGui->LinkEvent(
			mWindow->getTitlebar()->getName(), 
			new CEGUI::Event::Subscriber(&CEGuiDebuggingTools::TitlebarDoubleClicked, this), 
			CEGUI::Titlebar::EventMouseDoubleClick);

		mDebugRoot->addChildWindow(mWindow);
	}

	void CEGuiDebuggingTools::RegisterTool( std::string name, std::string buttonText, CEGuiDebuggerTool* tool )
	{
		tool->Initialize(mGui, mSkinFile, mDebugRoot);

		/*CEGUI::ButtonBase *button =  (CEGUI::ButtonBase*)CEGUI::WindowManager::getSingletonPtr()->createWindow(mSkinFile + "/Button",  "DebugingToolsButton" + name);
		button->setText(buttonText);
		button->setSize(CEGUI::UVector2(CEGUI::UDim(0, mButtonWidth), CEGUI::UDim(0, mButtonHeight)));
		button->setYPosition(CEGUI::UDim(0, 30));
		CEGUI::String fullname = "DebugingToolsButton" + name;
		mGui->LinkEvent(fullname, new CEGUI::Event::Subscriber(&CEGuiDebuggingTools::ButtonClicked, this));
		
	
		mWindow->addChildWindow(button);*/

		//Chris: updated this function to use components create button method
		CEGUI::ButtonBase *button = mGui->CreateButtonA("DebugingToolsButton" + name,
														buttonText,
														mButtonWidth,mButtonHeight,
														0,0.5,
														new CEGUI::Event::Subscriber(&CEGuiDebuggingTools::ButtonClicked, this));
		mWindow->addChildWindow(button);
		mTools.insert(std::make_pair(name, std::make_pair(button, tool)));

		RejigPositioning();
	}

	void CEGuiDebuggingTools::DestroyTool( std::string name )
	{
		CEGuiDebuggerTool *tool = mTools[name].second;

		tool->ShutDown();
		delete tool;

		CEGUI::WindowManager::getSingletonPtr()->destroyWindow(mTools[name].first->getName());
		mTools.erase(name);

		RejigPositioning();
	}

	void CEGuiDebuggingTools::RejigPositioning()
	{
		int count = 0;

		for(ToolsMapIt it = mTools.begin();
			it != mTools.end(); ++it)
		{
			CEGUI::ButtonBase *button = it->second.first;

			button->setXPosition(CEGUI::UDim(0, (mButtonWidth * count) + (mSpacing * count) + (mSpacing * 2)));

			++count;
		}
	}

	bool CEGuiDebuggingTools::ButtonClicked( const CEGUI::EventArgs &args )
	{
		const CEGUI::WindowEventArgs& we = static_cast<const CEGUI::WindowEventArgs&>(args);

		for(ToolsMapIt it = mTools.begin();
			it != mTools.end(); ++it)
		{
			if(it->second.first == (CEGUI::ButtonBase*)we.window)
			{
				if(it->second.second->IsVisible())
					it->second.second->Hide();
				else
				{
					it->second.second->Show();
				}

				return true;
			}
		}

		return false;
	}

	bool CEGuiDebuggingTools::TitlebarDoubleClicked( const CEGUI::EventArgs &args )
	{

		return true;
	}
}
