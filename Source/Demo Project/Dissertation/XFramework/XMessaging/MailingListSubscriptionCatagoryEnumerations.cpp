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
#include "MailingListSubscriptionCatagoryEnumerations.h"

namespace XF
{
	char* enumMailingListStrings[] = {
		"AUDIO_SUBSYSTEM",
		"PARTICLE_SUBSYSTEM",
		"GRAPHICS_SUBSYSTEM",
		"GUI_SUBSYSTEM",
		"ENITY_MANAGEMENT",
		"INTERESTED_IN_SETTINGS",
		"CONSOLE",
		"PATH_FOLLOWERS",

		"COLLISION_SYSTEM",
	};
	std::string MailingListToString(MailingList::ENUM sub){return enumMailingListStrings[sub];}
	MailingList::ENUM StringToMailingList(std::string message)
	{
		int resultm = NO_MAILING_LIST; 
		for(int i = 0; enumMailingListStrings[i] != "NULL"; ++i)
		{
			if(enumMailingListStrings[i] == message) return (MailingList::ENUM)i;
		}
		return (MailingList::ENUM)resultm;
	}
}
