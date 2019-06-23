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
#include <list>
#include <set>
#include <algorithm>

#include "XSystemComponents\SystemComponent.h"
#include "MailingListSubscriptionCatagoryEnumerations.h"
#include "Message.h"

namespace XF
{
	class Messenger;

	class PostOffice : public SystemComponent
	{
	private:
		typedef std::map<XF::MailingList::ENUM, std::list<int>> MailingListMap;
		typedef std::map<XF::MailingList::ENUM, std::list<int>>::const_iterator MailingListMapIterator;
		typedef std::map<std::string, int> PhonebookMap;

		MailingListMap mMailingListSubscriptions;
		std::map<int, Messenger*> mAllMessengers;
		std::set<Message> mPendingMessages;
		PhonebookMap mPhonebook;

		float mTime;

		void Dispatch(const Message &message);

	public:
		PostOffice(Application *ownerApp);
		~PostOffice();

		void RegisterInPhonebook(std::string name, int id);
		void UnregisterFromPhonebook(std::string name);
		int LookupPhonebook(std::string name);
		bool LookupPhonebookSafe( std::string name, int &id ); //chris for looking up in run window

		void SubscribeToMailingList(XF::MailingList::ENUM mlist, Messenger *messenger);
		void UnsubscribeFromMailingList(XF::MailingList::ENUM mList, Messenger *messenger);

		void PostAMessage(int receiverID, int senderID, MessageSubject::ENUM subject, void* data, float sendDelay);
		void PostAMessage(XF::MailingList::ENUM mailingList, int senderID, MessageSubject::ENUM subject, void* data, float sendDelay);

		void RegisterMessenger(Messenger *messenger);
		void UnregisterMessenger(Messenger *messenger);

		void OnInitialise();
		void OnShutDown();
		void OnUpdate(double dT);
		
	};
}
