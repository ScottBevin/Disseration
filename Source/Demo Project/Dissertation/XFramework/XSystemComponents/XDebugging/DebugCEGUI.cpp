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
#include "DebugCEGUI.h"
#include "OgreVector3.h"
#include "XFramework.h"

void DebugCEGUI::Log( std::string output )
{
	if(!mDebugBox)
	{
		XF::Logger::GetInstance()->Log("Error Debug Not Initialized");
		throw "Error Debug Not Initialized";
	}
	if(mEnabled)
	{
		if((int)mDebugBox->getItemCount() > mKeepThreshold)
		{
			if(mInverted)
			{
				mDebugBox->getListboxItemFromIndex(mKeepThreshold)->setAutoDeleted(true);
				mDebugBox->removeItem(mDebugBox->getListboxItemFromIndex(mKeepThreshold));			
			}			
			else
			{
				mDebugBox->removeItem(0);
				mDebugBox->getListboxItemFromIndex(0)->setAutoDeleted(true);
				mDebugBox->removeItem(mDebugBox->getListboxItemFromIndex(0));
			}


		}

		CEGUI::ListboxTextItem* lItem = new CEGUI::ListboxTextItem(output);
		if(mInverted)
			mDebugBox->insertItem(lItem,0);
		else
			mDebugBox->addItem(lItem);

		
		mDebugBox->getVertScrollbar()->setScrollPosition(mDebugBox->getTotalItemsHeight());

	}


}
void DebugCEGUI::Initialize(std::string skin , std::string WindowTag )
{
	mWindowManager = NULL;
	std::string listBoxSkin =skin/*+"skin"*/+"/Listbox";
	std::string frameSkin =skin/*+"skin"*/+"/FrameWindow";
	mWindowManager =  CEGUI::WindowManager::getSingletonPtr();
	CEGUI::FrameWindow* frameWindow = (CEGUI::FrameWindow*)CEGUI::WindowManager::getSingletonPtr()->createWindow(frameSkin, "DebugFrameWindow");
	mDebugBox = (CEGUI::Listbox*)CEGUI::WindowManager::getSingletonPtr()->createWindow(listBoxSkin, "Debug");

	mDebugBox->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	mDebugBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0,0)));
	mDebugBox->setAlpha(1);

	frameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.15, 0)));
	frameWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0,0)));
	frameWindow->setAlpha(0.5);
	
	mWindowManager->getWindow("DebugFrameWindow")->addChildWindow("Debug");
	mWindowManager->getWindow(WindowTag)->addChildWindow("DebugFrameWindow");

	XF_LOGGER->OnLogMessage.RegisterMethod<DebugCEGUI, &DebugCEGUI::OnMessage>(this);
}
DebugCEGUI::~DebugCEGUI()
{
	mWindowManager->destroyWindow("Debug");
	mWindowManager->destroyWindow("DebugFrameWindow");
}

void DebugCEGUI::OnMessage( std::string message )
{
	Log(message);
}

