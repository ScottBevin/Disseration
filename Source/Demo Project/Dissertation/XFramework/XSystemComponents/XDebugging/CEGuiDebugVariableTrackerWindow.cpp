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


#include "CEGuiDebugVariableTrackerWindow.h"
#include "DebugVariableTracker.h"

namespace XF
{
	CEGuiDebugVariableTrackerWindow::CEGuiDebugVariableTrackerWindow() 
		: mWidth(300), mVerticalSpacing(5), mHorizontalSpacing(5), mTextHeight(20)
	{
	}

	CEGuiDebugVariableTrackerWindow::~CEGuiDebugVariableTrackerWindow()
	{
	}

	void CEGuiDebugVariableTrackerWindow::OnInitialize()
	{
		DebugVariableTracker::OnVariableRegisteredEvent.RegisterMethod<CEGuiDebugVariableTrackerWindow, &CEGuiDebugVariableTrackerWindow::OnVariableRegistered>(this);
		DebugVariableTracker::OnVariableChangedEvent.RegisterMethod<CEGuiDebugVariableTrackerWindow, &CEGuiDebugVariableTrackerWindow::OnVariableChanged>(this);
		DebugVariableTracker::OnVariableUnregisteredEvent.RegisterMethod<CEGuiDebugVariableTrackerWindow, &CEGuiDebugVariableTrackerWindow::OnVariableUnregistered>(this);

		mDebugWindow = (CEGUI::FrameWindow*)CEGUI::WindowManager::getSingletonPtr()->createWindow(mSkin + "/FrameWindow", "DebugVariableTrackerWindow");
		mDebugWindow->setText("Tracked Variables");
		mDebugWindow->setVisible(false);
		mDebugWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 50), CEGUI::UDim(0, 50)));
		mDebugWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, mWidth), CEGUI::UDim(0, 100)));
		mDebugWindow->setAlpha(0.5f);
		mDebugWindow->setCloseButtonEnabled(true);
		mDebugWindow->setHeight(CEGUI::UDim(0, ((mVariables.size() + 2) * mTextHeight) + (mVerticalSpacing * 2)));
		mDebugWindow->setAlwaysOnTop(true);
		
		mGui->LinkEvent(
			mDebugWindow->getCloseButton()->getName(), 
			new CEGUI::Event::Subscriber(&CEGuiDebuggerTool::CloseWindowClick, (CEGuiDebuggerTool*)this));

		mRootWindow->addChildWindow(mDebugWindow);

		RejigWindowHeight();
	}

	void CEGuiDebugVariableTrackerWindow::OnShutDown()
	{
		CEGUI::WindowManager::getSingletonPtr()->destroyWindow("DebugVariableTrackerWindow");

		DebugVariableTracker::OnVariableRegisteredEvent.UnregisterMethod<CEGuiDebugVariableTrackerWindow, &CEGuiDebugVariableTrackerWindow::OnVariableRegistered>(this);
		DebugVariableTracker::OnVariableChangedEvent.UnregisterMethod<CEGuiDebugVariableTrackerWindow, &CEGuiDebugVariableTrackerWindow::OnVariableChanged>(this);
		DebugVariableTracker::OnVariableUnregisteredEvent.UnregisterMethod<CEGuiDebugVariableTrackerWindow, &CEGuiDebugVariableTrackerWindow::OnVariableUnregistered>(this);
	}

	void CEGuiDebugVariableTrackerWindow::OnVariableRegistered( std::string name )
	{
		int count = (int)mVariables.size();
		float top = (count * mTextHeight) + mVerticalSpacing + 20;
		float buttonWidth = (mWidth * 0.5f) - (mHorizontalSpacing * 4);

		CEGUI::ButtonBase *text = (CEGUI::ButtonBase*)CEGUI::WindowManager::getSingletonPtr()->createWindow(mSkin + "/Button",  "DebugVariable" + name + "text");
		text->setText(name);
		text->setXPosition(CEGUI::UDim(0.05, 0));
		text->setWidth(CEGUI::UDim(0.44, 0));
		text->setHeight(CEGUI::UDim(0, mTextHeight));
		mDebugWindow->addChildWindow(text);

		CEGUI::ButtonBase *variable = (CEGUI::ButtonBase*)CEGUI::WindowManager::getSingletonPtr()->createWindow(mSkin + "/Button",  "DebugVariable" + name + "variable");
		variable->setText("Not set");
		variable->setXPosition(CEGUI::UDim(0.51, 0));
		variable->setSize(CEGUI::UVector2(CEGUI::UDim(0.44, 0), CEGUI::UDim(0, mTextHeight)));
		mDebugWindow->addChildWindow(variable);

		mVariables.push_back(std::make_pair(name, std::make_pair(text, variable)));

		RejigWindowHeight();
	}

	void CEGuiDebugVariableTrackerWindow::OnVariableChanged( std::string name, std::string value )
	{
		for(VariableVecIt it = mVariables.begin(); it != mVariables.end(); ++it)
		{
			if(it->first == name)
			{
				it->second.second->setText(value);
				break;
			}
		}		
	}

	void CEGuiDebugVariableTrackerWindow::OnVariableUnregistered( std::string name )
	{
		CEGUI::WindowManager::getSingletonPtr()->destroyWindow("DebugVariable" + name + "text");
		CEGUI::WindowManager::getSingletonPtr()->destroyWindow("DebugVariable" + name + "variable");

		for(VariableVecIt it = mVariables.begin(); it != mVariables.end(); ++it)
		{
			if(it->first == name)
			{
				mVariables.erase(it);
				break;
			}
		}

		RejigWindowHeight();
	}

	void CEGuiDebugVariableTrackerWindow::RejigWindowHeight()
	{
		int count = 0;

		for(VariableVecIt it = mVariables.begin();
			it != mVariables.end(); ++it)
		{
			float top = ((count + 1) * mTextHeight) + (mVerticalSpacing * (count + 1));

			CEGUI::ButtonBase *text = it->second.first;
			text->setYPosition(CEGUI::UDim(0, top));

			CEGUI::ButtonBase *variable = it->second.second;
			variable->setYPosition(CEGUI::UDim(0, top));		

			++count;
		}

		CEGUI::UDim height = CEGUI::UDim(0, ((mVariables.size() + 1) * mTextHeight) + (mVerticalSpacing * (mVariables.size() + 2)));

		mDebugWindow->setHeight(height);
		mDebugWindow->setMinSize(CEGUI::UVector2(CEGUI::UDim(0, mWidth), height));
		mDebugWindow->setMaxSize(CEGUI::UVector2(CEGUI::UDim(1, 0), height));
	}
}