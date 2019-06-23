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

#include "Messenger.h"

namespace XF
{
	PostOffice *Messenger::msPostOffice = NULL;
	int Messenger::msNextUID = 1;

	Messenger::Messenger()
	{
		mUID = msNextUID;
		msNextUID++;

		if(msPostOffice)
			msPostOffice->RegisterMessenger(this);
	}

	Messenger::~Messenger()
	{
		if(msPostOffice)
			msPostOffice->UnregisterMessenger(this);
	}

	void Messenger::SubscribeToMailingList( MailingList::ENUM mList )
	{
		if(msPostOffice)
			msPostOffice->SubscribeToMailingList(mList, this);
	}

	void Messenger::UnsubscribeFromMailingList( MailingList::ENUM mList )
	{
		if(msPostOffice)
			msPostOffice->UnsubscribeFromMailingList(mList, this);
	}

	void Messenger::PostAMessage( int receiverID, MessageSubject::ENUM subject, void* data /*= NULL*/, float sendDelay/* = 0*/ )
	{
		if(msPostOffice)
			msPostOffice->PostAMessage(receiverID, mUID, subject, data, sendDelay);
	}

	void Messenger::PostAMessage( MailingList::ENUM mailingList, MessageSubject::ENUM subject, void* data /*= NULL*/, float sendDelay/* = 0*/)
	{
		if(msPostOffice)
			msPostOffice->PostAMessage(mailingList, mUID, subject, data, sendDelay);
	}

	void Messenger::RegisterInPhonebook( std::string name )
	{
		mPhonebookName = name;
		msPostOffice->RegisterInPhonebook(name, ID());
	}

	void Messenger::UnregisterFromPhonebook()
	{
		msPostOffice->UnregisterFromPhonebook(mPhonebookName);
	}

	int Messenger::LookupPhonebook( std::string name )
	{
		return msPostOffice->LookupPhonebook(name);
	}
	bool Messenger::LookupPhonebookSafe( std::string name,int& id )
	{
		return msPostOffice->LookupPhonebookSafe(name,id);
	}
}
