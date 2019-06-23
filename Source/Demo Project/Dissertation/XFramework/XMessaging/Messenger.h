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

#include "Message.h"
#include "PostOffice.h"

namespace XF
{
	class Messenger
	{
	private:
		static PostOffice *msPostOffice;
		static int msNextUID;
		std::string mPhonebookName;

		int mUID;

	protected:
		virtual bool OnHandleMessage(const Message &message) { return false; }

	public:
		Messenger();
		virtual ~Messenger();

		bool HandleMessage(const Message &message){ return OnHandleMessage(message); }
		
		bool MessageSelf(MessageSubject::ENUM subject, void* data = NULL)//Chris Added this function for streamlining component internal message handling
		{
			Message message;
			message.ReceiverID = mUID;
			message.SenderID = mUID;
			message.SendTime = 0;
			message.Subject = subject;
			message.Data = data;
			return OnHandleMessage(message);
		}

	
		void PostAMessage(int receiverID, MessageSubject::ENUM subject, void* data = NULL, float sendDelay = 0); //chris i just added default parameters to clean things up a little and make them easier
	
		void PostAMessage(XF::MailingList::ENUM mailingList, MessageSubject::ENUM subject, void* data = NULL, float sendDelay = 0);
		

		void SubscribeToMailingList(XF::MailingList::ENUM mList);
		void UnsubscribeFromMailingList(XF::MailingList::ENUM mList);

		void RegisterInPhonebook(std::string name);
		void UnregisterFromPhonebook();
		int LookupPhonebook(std::string name);
		bool LookupPhonebookSafe( std::string name,int& id ); //chris

		int ID(){return mUID;}

		static void PostOffice(PostOffice * val) 
		{ 
			msPostOffice = val; 
		}
		
	};
}
