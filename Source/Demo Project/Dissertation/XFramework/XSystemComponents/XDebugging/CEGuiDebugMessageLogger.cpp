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


#include "CEGuiDebugMessageLogger.h"
#include "..\..\XUtilities\Logger.h"

namespace XF
{
	CEGuiDebugMessageLogger::CEGuiDebugMessageLogger()
	{
	}

	CEGuiDebugMessageLogger::~CEGuiDebugMessageLogger()
	{
	}

	void CEGuiDebugMessageLogger::OnInitialize()
	{
		XF_LOGGER->OnLogMessage.RegisterMethod<CEGuiDebugMessageLogger, &CEGuiDebugMessageLogger::OnMessageLog>(this);

		mDebugWindow = (CEGUI::FrameWindow*)CEGUI::WindowManager::getSingletonPtr()->createWindow(mSkin + "/FrameWindow", "DebugLoggerWindow");
		mDebugWindow->setText("Message Logger");
		mDebugWindow->setVisible(false);
		mDebugWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 100)));
		mDebugWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0, 200)));
		mDebugWindow->setAlpha(0.5f);
		mDebugWindow->setCloseButtonEnabled(true);
		mDebugWindow->setAlwaysOnTop(true);

		mGui->LinkEvent(
			mDebugWindow->getCloseButton()->getName(), 
			new CEGUI::Event::Subscriber(&CEGuiDebuggerTool::CloseWindowClick, (CEGuiDebuggerTool*)this));

		mDebugListBox = (CEGUI::Listbox*)CEGUI::WindowManager::getSingletonPtr()->createWindow(mSkin + "/Listbox", "DebugLoggerListBox");
		mDebugListBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.01, 0), CEGUI::UDim(0.12, 0)));
		mDebugListBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.98, 0), CEGUI::UDim(0.87, 0)));
		mDebugWindow->addChildWindow(mDebugListBox);

		this->mRootWindow->addChildWindow(mDebugWindow);

		for(int i = 0; i < XF_CEGUIDEBUGMESSAGELOGGER_HISTORY_COUNT; ++i)
		{
			CEGUI::ListboxTextItem* lItem = new CEGUI::ListboxTextItem("");
			mDebugListBox->addItem(lItem);
			mListboxItems[i] = lItem;
		}
	}

	void CEGuiDebugMessageLogger::OnShutDown()
	{
		for(int i = 0; i < XF_CEGUIDEBUGMESSAGELOGGER_HISTORY_COUNT; ++i)
		{
			mDebugListBox->removeItem(mListboxItems[i]);
		}

		CEGUI::WindowManager::getSingletonPtr()->destroyWindow("DebugLoggerListBox");
		CEGUI::WindowManager::getSingletonPtr()->destroyWindow("DebugVariableTrackerWindow");

		XF_LOGGER->OnLogMessage.UnregisterMethod<CEGuiDebugMessageLogger, &CEGuiDebugMessageLogger::OnMessageLog>(this);

	}

	void CEGuiDebugMessageLogger::OnMessageLog( std::string message )
	{
		for(int i = 0; i < XF_CEGUIDEBUGMESSAGELOGGER_HISTORY_COUNT - 1; ++i)
		{
			mListboxItems[i]->setText(mListboxItems[i + 1]->getText());
		}

		mListboxItems[XF_CEGUIDEBUGMESSAGELOGGER_HISTORY_COUNT - 1]->setText(message);

		mDebugListBox->getVertScrollbar()->setScrollPosition(mDebugListBox->getTotalItemsHeight());
		mDebugListBox->handleUpdatedItemData();
	}
}
