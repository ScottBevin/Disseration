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

#include "PostOffice.h"

#include "Messenger.h"
#include "XUtilities/Logger.h"
#include "XSystemComponents/XDebugging/DebugVariableTracker.h"

namespace XF
{
	PostOffice::PostOffice(Application *ownerApp) : SystemComponent(ownerApp)
	{
		Messenger::PostOffice(this);
	}

	PostOffice::~PostOffice()
	{
	}

	void PostOffice::SubscribeToMailingList( MailingList::ENUM mlist, Messenger *messenger )
	{
		this->mMailingListSubscriptions[mlist].push_back(messenger->ID());

		LOG << "[XF::PostOffice] - Agent: " << messenger->ID() << " subscribed to mailing list: " << (int)mlist;
	}

	void PostOffice::UnsubscribeFromMailingList( MailingList::ENUM mList, Messenger *messenger )
	{
		if(mMailingListSubscriptions.find(mList) != mMailingListSubscriptions.end())
		{
			std::list<int>::iterator it = std::find(
				mMailingListSubscriptions[mList].begin(),
				mMailingListSubscriptions[mList].end(),
				messenger->ID());

			if(it != mMailingListSubscriptions[mList].end())
				mMailingListSubscriptions[mList].erase(it);

			LOG << "[XF::PostOffice] - Agent: " << messenger->ID() << " unsubscribed to mailing list: " << (int)mList;
		}
	}

	void PostOffice::PostAMessage( int receiverID, int senderID, MessageSubject::ENUM subject, void* data, float sendDelay )
	{
		Message message;

		message.ReceiverID = receiverID;
		message.SenderID = senderID;
		message.SendTime = mTime + sendDelay;
		message.Subject = subject;
		message.Data = data;

		if(sendDelay == 0)
			Dispatch(message);
		else
			this->mPendingMessages.insert(message); 
	}

	void PostOffice::PostAMessage( MailingList::ENUM mailingList, int senderID, MessageSubject::ENUM subject, void* data, float sendDelay )
	{
		if(mMailingListSubscriptions.find(mailingList) != mMailingListSubscriptions.end())
		{
			for(
				std::list<int>::iterator it = this->mMailingListSubscriptions[mailingList].begin();
				it != mMailingListSubscriptions[mailingList].end();
				++it)
				{
					int id = (*it);

					PostAMessage(id, senderID, subject, data, sendDelay);
				}
		}
	}

	void PostOffice::RegisterMessenger( Messenger *messenger )
	{
		mAllMessengers.insert(std::make_pair(messenger->ID(), messenger));
	}

	void PostOffice::UnregisterMessenger( Messenger *messenger )
	{
		mAllMessengers.erase(messenger->ID());
	}

	void PostOffice::OnInitialise()
	{
		mTime = 0;
	}

	void PostOffice::OnShutDown()
	{
		Messenger::PostOffice(NULL);
	}

	void PostOffice::OnUpdate(double dT)
	{
		mTime += dT;

		while (mPendingMessages.begin()!= mPendingMessages.end() &&
			mPendingMessages.begin()->SendTime < mTime &&
			mPendingMessages.begin()->SendTime > 0)
		{
			Message message = *mPendingMessages.begin();

			Dispatch(message);
	
			mPendingMessages.erase(mPendingMessages.begin());
		}
	}

	void PostOffice::Dispatch(const  Message &message ) //this could be a bool?
	{
		std::map<int, Messenger*>::iterator it = this->mAllMessengers.find(message.ReceiverID);

		if(it != mAllMessengers.end())
		{
			if(it->second->HandleMessage(message))
			{
				LOG << "[XF::PostOffice] - Agent: " << message.ReceiverID << " handled message: " << MessageSubjectToString(message.Subject);
			}
			else
			{
				ILOG << "[XF::PostOffice] - Agent: " << message.ReceiverID << " failed to handle message: " << MessageSubjectToString(message.Subject); 
			}
		}
	}

	void PostOffice::RegisterInPhonebook( std::string name, int id )
	{
		mPhonebook[name] = id;
		LOG << "[XF::PostOffice] - Agent: " << id << " registered in phonebook as: " << name;
	}

	void PostOffice::UnregisterFromPhonebook( std::string name)
	{
		mPhonebook.erase(name);
		LOG << "[XF::PostOffice] - " << name << " unregistered from phonebook";
	}

	int PostOffice::LookupPhonebook( std::string name )
	{
		return mPhonebook[name];
	}

	bool PostOffice::LookupPhonebookSafe( std::string name, int & id )
	{
		PhonebookMap::iterator it = mPhonebook.find(name);
		if(it == mPhonebook.end()) return false;
		
		id = mPhonebook[name];
		return true;
	}
}