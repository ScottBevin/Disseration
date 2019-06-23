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
#include "CEGUIDebuggerTool.h"
#include <vector>
#include <map>
#include <string>
#include "../../XDelegates/Delegate.h"
#include "XUtilities/Logger.h"
#include "XUtilities/Utilities.h"

namespace XF
{
	
	class CEGUIFunctionRunner :
		public CEGuiDebuggerTool, Messenger
	{
	public:	
		typedef Delegate1<bool, std::vector<std::string> & > DelegateType;
	private:
		
		int mNumber;
		
		static std::map<std::string, DelegateType > smFunctions;
		static std::map<std::string, std::string > smDescriptions;

		bool PostMessageAgent(std::vector<std::string> & args);
		bool PostMessageMailingList(std::vector<std::string> & args);
		bool PostMessagePhoneBook(std::vector<std::string> & args);
		bool ShowAllCommands(std::vector<std::string> & args);
		bool ShowAllMessages(std::vector<std::string> & args);
		bool ShowAllMailingList(std::vector<std::string> & args);
		bool ChangeConsoleMemory(std::vector<std::string>& args);
		bool Clear(const CEGUI::EventArgs& args);

		static XF::DebugDisplay* smDisplay; //might aswell reuse this for this
	protected:
		CEGUI::Editbox * mTextBox;
		CEGUI::Listbox * mOldText;
		CEGUI::Window* mFunctionButton;
		CEGUI::Window* mFrameWindow;

		void OnInitialize();
		void OnUpdate(double dT);
		void OnHandleInput(Input *input, double dT);
		void OnShutDown();
		bool FunctionButtonPressed(const CEGUI::EventArgs &args);

	
		bool OnHandleMessage(const Message &message);
	public:	
	
		
		
		static void RegisterFunction(std::string key, std::string description, DelegateType functionv)
		{
			key = XF::Utilities::ToLower(key);
		
			smFunctions[key] = functionv;
			smDescriptions[key] = description;
		}
		static void UnRegisterFunction( std::string key )
		{
			smFunctions.erase(key);
		}
	
	/*
	
			template<typename T,typename (T::*function)(std::vector<std::string> &)>
				static void RegisterFunctionB(std::string key, std::string description, T* obj)
				{
					smFunctions[key] = DelegateType::FromMethod<T,function>(obj);
				}*/
	
	
	


		void Show();
		void Hide();
		bool IsVisible();

		static XF::DebugDisplay* GetDisplay(){return smDisplay;};
		CEGUIFunctionRunner();
		~CEGUIFunctionRunner();
		

		
	};
}