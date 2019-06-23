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
#include "CEGUIFunctionRunner.h"

#include "../../XUtilities/Utilities.h"
#include "DebugCEGUIListBox.h"
#include "XMessaging/MailingListSubscriptionCatagoryEnumerations.h"

namespace XF
{
	XF::DebugDisplay* CEGUIFunctionRunner::smDisplay = NULL;
	std::map<std::string,  CEGUIFunctionRunner::DelegateType > CEGUIFunctionRunner::smFunctions;
	std::map<std::string, std::string > CEGUIFunctionRunner::smDescriptions;
	CEGUIFunctionRunner::CEGUIFunctionRunner()
	{

	}

	CEGUIFunctionRunner::~CEGUIFunctionRunner()
	{
	}

	bool CEGUIFunctionRunner::PostMessageMailingList(std::vector<std::string> & args)
	{
		if(args.size() < 2)
		{XF::DebugMessage(smDisplay) << "ERROR: Not enough arguments!"; return false;}
		std::string mailingList = args[0];
		std::string MessageAsString = args[1];
		args.erase(args.begin());
		args.erase(args.begin());
		int mlList;
		
		XF::Utilities::FromString<int>(mlList, mailingList);
		XF::MailingList::ENUM mailTo = (XF::MailingList::ENUM)mlList;
		
		XF::MessageSubject::ENUM messSubject = XF::MessageSubject::StringToMessageSubject(MessageAsString);

		if(messSubject == NO_MESSAGE)
		{
			XF::DebugMessage(smDisplay) << "ERROR: No such message: "<<MessageAsString;
			return false;
		}
		else
		{
			XF::DebugMessage(smDisplay) << "Posting message: "<< MessageAsString << " To Mailing list "<< mailingList << ":" << mailTo;
		}
		if(args[0] == "int") 
		{
			int val = 0;
			XF::Utilities::FromString<int>(val,args[1]);
			this->PostAMessage(mailTo, messSubject, &val);
		}
		else if(args[0] == "float") 
		{
			float val = 0;
			XF::Utilities::FromString<float>(val,args[1]);
				this->PostAMessage(mailTo, messSubject, &val);
		}
		else if(args[0] == "vector") 
		{
			XF::Vector3 val = XF::Utilities::FromString(args[1]);
			this->PostAMessage(mailTo, messSubject, &val);
		}
		else if(args[0] == "string") 
		{
			this->PostAMessage(mailTo, messSubject, &args[1]);
		}

		this->PostAMessage(mailTo, messSubject, &args);
		return true;
	}

	bool CEGUIFunctionRunner::PostMessagePhoneBook(std::vector<std::string> & args)
	{
		int id = 0;
		std::string phonebookName = args[0];
		if(!LookupPhonebookSafe(phonebookName,id))
		{			
			XF::DebugMessage(smDisplay) << "ERROR: No such id: "+args[0];
			
			return false;
		}
		if(args.size() < 2)
		{XF::DebugMessage(smDisplay) << "ERROR: Not enough arguments!"; return false;}
		std::string MessageAsString = args[1];
		args.erase(args.begin());
		args.erase(args.begin());
	
		
		XF::MessageSubject::ENUM messSubject = XF::MessageSubject::StringToMessageSubject(MessageAsString);

		if(messSubject == NO_MESSAGE)
		{
			XF::DebugMessage(smDisplay) << "ERROR: No such message: " << MessageAsString;
			return false;
		}
		else
		{
			XF::DebugMessage(smDisplay) << "Posting message: "<< MessageAsString << " To Agent "<< id << " AKA " << phonebookName;
		}

		if(args[0] == "int") 
		{
			int val = 0;
			XF::Utilities::FromString<int>(val,args[1]);
			this->PostAMessage(id, messSubject, &val);
		}
		else if(args[0] == "float") 
		{
			float val = 0;
			XF::Utilities::FromString<float>(val,args[1]);
				this->PostAMessage(id, messSubject, &val);
		}
		else if(args[0] == "vector") 
		{
			XF::Vector3 val = XF::Utilities::FromString(args[1]);
			this->PostAMessage(id, messSubject, &val);
		}
		else if(args[0] == "string") 
		{
			this->PostAMessage(id, messSubject, &args[1]);
		}

		this->PostAMessage(id, messSubject, &args);
			
		
		return true;
	}
	bool CEGUIFunctionRunner::PostMessageAgent(std::vector<std::string> & args)
	{
		if(args.size() < 2)
		{XF::DebugMessage(smDisplay) << "ERROR: Not enough arguments!"; return false;}

		std::string SentToIds = args[0];
		std::string MessageAsString = args[1];
		args.erase(args.begin());
		args.erase(args.begin());
		int SendID;
		
		XF::Utilities::FromString<int>(SendID, SentToIds);
		
		 XF::MessageSubject::ENUM messSubject = XF::MessageSubject::StringToMessageSubject(MessageAsString);

		if(messSubject == NO_MESSAGE)
		{
			XF::DebugMessage(smDisplay) << "ERROR: No such message: " << MessageAsString;
			return false;
		}
		else
		{
			XF::DebugMessage(smDisplay) << "Posting message: "<< MessageAsString << " To Agent "<< SendID;
		}
		if(args[0] == "int") 
		{
			int val = 0;
		    XF::Utilities::FromString<int>(val,args[1]);
			this->PostAMessage(SendID, messSubject, &val);
		}
		else if(args[0] == "float") 
		{
			float val = 0;
			XF::Utilities::FromString<float>(val,args[1]);
				this->PostAMessage(SendID, messSubject, &val);
		}
		else if(args[0] == "vector") 
		{
			XF::Vector3 val = XF::Utilities::FromString(args[1]);
			
			this->PostAMessage(SendID, messSubject, &val);
		}
		else if(args[0] == "string") 
		{
			this->PostAMessage(SendID, messSubject, &args[1]);
		}


		this->PostAMessage(SendID, messSubject, &args);
		return true;
	}
	bool CEGUIFunctionRunner::FunctionButtonPressed(const CEGUI::EventArgs &args)
	{
		std::string text = mTextBox->getText().c_str();
		
		text = XF::Utilities::ToLower(text);

		XF::DebugMessage(smDisplay) << mNumber << ":"<< text;
		mNumber++;
		
		std::vector<std::string> strings;
		XF::Utilities::TokenizeString(text,	strings);
		if(strings.empty())return false;
		std::string functionKey = strings[0];
		strings.erase(strings.begin());

		std::map<std::string, DelegateType >::iterator it;
		it = smFunctions.find(functionKey);
		if(it == smFunctions.end())
		{		
			XF::DebugMessage(smDisplay) << ("ERROR: No such Function key: "+functionKey);			
			return false;
		}

		if(it->second(strings))
		Clear(args);
		return true;

	}
	bool CEGUIFunctionRunner::ShowAllCommands(std::vector<std::string> & args)
	{
		std::map<std::string, DelegateType >::iterator it;
		for(it= smFunctions.begin(); it != smFunctions.end(); ++it)
		{	
			XF::DebugMessage(smDisplay) << "'" << it->first << "'                 :" << smDescriptions[it->first];
		}
		return true;
	}
	bool CEGUIFunctionRunner::ShowAllMessages(std::vector<std::string> & args)
	{
		std::string result = "0"; 
		for(int i = 0; result != "NULL"; ++i)
		{
			result = XF::MessageSubject::MessageSubjectToString((XF::MessageSubject::ENUM)i);
			XF::DebugMessage(smDisplay) <<result;

		}
		return true;
	}
	bool CEGUIFunctionRunner::ShowAllMailingList(std::vector<std::string> & args)
	{
		std::string result = "0"; 
		for(int i = 0; result != "NULL"; ++i)
		{
			result = MailingListToString((XF::MailingList::ENUM)i);
			XF::DebugMessage(smDisplay) <<result;

		}
		return true;
	}
	bool CEGUIFunctionRunner::Clear(const CEGUI::EventArgs& args)
	{
		mTextBox->setText("");
		return true;
	}
	bool CEGUIFunctionRunner::ChangeConsoleMemory(std::vector<std::string>& args)
	{
		int num;
		if(XF::Utilities::FromString<int>(num,args[0]))		
		{
			(( XF::DebugCEGUIListBox*)(smDisplay))->SetKeepThreshold(num);
					return true;
		}
		return false;
	}
	
	void CEGUIFunctionRunner::OnInitialize()
	{
		mNumber = 0;
		SubscribeToMailingList(XF::MailingList::CONSOLE);
		mTextBox = (CEGUI::Editbox *)mGui->CreateAControl("ConsoleEditbox","Editbox");
		mOldText= (CEGUI::Listbox *)mGui->CreateAControl("ConsoleListbox","Listbox");

		float oldTextYPos = 0.2;
		float oldTextHeight = 0.48;
		mOldText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.02,0),CEGUI::UDim(oldTextYPos,0.0)));
		mOldText->setWidth(CEGUI::UDim(0.95,0.0));
		mOldText->setHeight(CEGUI::UDim(oldTextHeight,0.0));
		
		float textyPos = oldTextYPos + oldTextHeight;
		mTextBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.02,0),CEGUI::UDim(textyPos,0.0)));
		mTextBox->setWidth(CEGUI::UDim(0.8,0.0));
		mTextBox->setHeight(CEGUI::UDim(0.2,0.0));
		
		mFunctionButton = mGui->CreateButtonB("clearFunction","Clear",0.15,0.2, 
				                                   mTextBox->getWidth().d_scale+0.01f,textyPos,
				new CEGUI::SubscriberSlot(&CEGUIFunctionRunner::Clear, this));
		
		
		mGui->LinkEvent(mTextBox,new CEGUI::SubscriberSlot(&CEGUIFunctionRunner::FunctionButtonPressed, this),CEGUI::Editbox::EventTextAccepted);
		
		mFrameWindow = mGui->CreateFrameWindow("FunctionRunnerWindow");
		mGui->LinkEvent(mFrameWindow, new CEGUI::SubscriberSlot(&CEGuiDebuggerTool::CloseWindowClick, (CEGuiDebuggerTool*)this),CEGUI::FrameWindow::EventCloseClicked);

		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.9,0),CEGUI::UDim(0.25,0.0)));
		mFrameWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.1,0.0)));
		mFrameWindow->setAlpha(0.6f);
		mFrameWindow->addChildWindow(mOldText);
		mFrameWindow->addChildWindow(mFunctionButton);
		mFrameWindow->addChildWindow(mTextBox);		
		this->mRootWindow->addChildWindow(mFrameWindow);
		
		RegisterFunction("PM","Post Message to an individual agent/screen/messenger with *SYNTAX*: PM agentNumber messageString dataVariableTYPE messageData", DelegateType::FromMethod< CEGUIFunctionRunner, &CEGUIFunctionRunner::PostMessageAgent  >(this));
		RegisterFunction("PMMailing","Post Message to a mailing list with *SYNTAX*: PMMailing MailingList::ENUMString messageString dataVariableTYPE messageData",DelegateType::FromMethod< CEGUIFunctionRunner, &CEGUIFunctionRunner::PostMessageMailingList  >(this));
		RegisterFunction("PMPhonebook","Post Message to an individual  agent/screen/messenger via phone book alias with *SYNTAX*: PMPhonebook agentPhonebookString messageString dataVariableTYPE messageData",DelegateType::FromMethod< CEGUIFunctionRunner, &CEGUIFunctionRunner::PostMessagePhoneBook  >(this));
		RegisterFunction("Messages","Show a list of all possible messages",DelegateType::FromMethod< CEGUIFunctionRunner, &CEGUIFunctionRunner::ShowAllMessages  >(this));
		RegisterFunction("Mail","Show a list of all possible mailing lists",DelegateType::FromMethod< CEGUIFunctionRunner, &CEGUIFunctionRunner::ShowAllMailingList  >(this));
		RegisterFunction("ConsoleMemory","change the amount that the console stores in its listbox",DelegateType::FromMethod< CEGUIFunctionRunner, &CEGUIFunctionRunner::ChangeConsoleMemory  >(this));

		RegisterFunction("Commands","list commands",DelegateType::FromMethod< CEGUIFunctionRunner, &CEGUIFunctionRunner::ShowAllCommands  >(this));
		RegisterFunction("?","shortcut for listing commands",DelegateType::FromMethod< CEGUIFunctionRunner, &CEGUIFunctionRunner::ShowAllCommands  >(this));

		if(!smDisplay)
			smDisplay = new XF::DebugCEGUIListBox(mOldText,15);
		
		XF::DebugMessage(smDisplay) << "Welcome to the console window =], type a command and hit enter! try '?' if you want a list of functions ";
		Hide();
	}
	void CEGUIFunctionRunner::Show()
	{
		mGui->ShowWindow("FunctionRunnerWindow");
		mGui->SetFocus(mTextBox);
		
	}
	void CEGUIFunctionRunner::Hide()
	{
		mGui->HideWindow("FunctionRunnerWindow");
	}
	bool CEGUIFunctionRunner::IsVisible()
	{
		return mFrameWindow->isVisible();
	}
	void CEGUIFunctionRunner::OnUpdate(double dT)
	{

	}
	void CEGUIFunctionRunner::OnHandleInput(Input *input, double dT)
	{

	}
	void CEGUIFunctionRunner::OnShutDown()
	{
		UnsubscribeFromMailingList(XF::MailingList::CONSOLE);
	}

	bool CEGUIFunctionRunner::OnHandleMessage( const Message &message )
	{
		switch(message.Subject)
		{
		case XF::MessageSubject::SEND_ME_DISPLAY:
			{
				PostAMessage(message.SenderID, XF::MessageSubject::SENT_CONSOLE_DISPLAY,smDisplay);
				return true;
			}		
		}
		return false;
	}


	
}
